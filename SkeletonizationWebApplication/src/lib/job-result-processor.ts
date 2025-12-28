import { createImage } from "@/repositories/image";
import { getJobsByRunId, getJobWithOwner, type JobWithOwner, updateJobStatus } from "@/repositories/job";
import { upsertJobStats } from "@/repositories/job-stats";
import { getRunOwnerAndName, getRunStatus, updateRunStatus } from "@/repositories/run";

import { copyOutputFile } from "./output-file-handler";
import { createQueueConsumer, QUEUE_NAMES, type SkeletonizationWorkerResult } from "./redis/index";
import { publishRunCompletedEvent } from "./run-events";

const checkRunCompletion = async (runId: string): Promise<void> => {
  const status = await getRunStatus(runId);
  if (status === "completed") return;

  const jobs = await getJobsByRunId(runId);
  const allComplete = jobs.every((j) => j.status === "completed" || j.status === "failed");

  if (!allComplete) return;

  const anyFailed = jobs.some((j) => j.status === "failed");
  const completedAt = new Date();

  const [runDetails] = await Promise.all([getRunOwnerAndName(runId), updateRunStatus(runId, "completed", completedAt)]);

  console.log(`Run ${runId} finished (anyFailed=${anyFailed})`);

  if (runDetails?.userId) {
    await publishRunCompletedEvent({
      runId,
      userId: runDetails.userId,
      runName: runDetails.name,
      status: "completed",
      jobCount: jobs.length,
      completedAt: completedAt.toISOString()
    });
  }
};

const processSuccessfulResult = async (jobInfo: JobWithOwner, outputPath: string): Promise<void> => {
  const fileInfo = await copyOutputFile(outputPath);

  await createImage({
    userId: jobInfo.userId!,
    originalFilename: fileInfo.originalFilename,
    storagePath: fileInfo.storagePath,
    url: fileInfo.url,
    mime: fileInfo.mime,
    width: fileInfo.width,
    height: fileInfo.height,
    sizeBytes: fileInfo.sizeBytes,
    checksum: fileInfo.checksum,
    status: "skeletonized",
    parentImageId: jobInfo.inputImageId,
    generatedByJobId: jobInfo.jobId
  });

  await updateJobStatus(jobInfo.jobId, "completed");
};

const processResult = async (result: SkeletonizationWorkerResult): Promise<void> => {
  const jobInfo = await getJobWithOwner(result.job_id);

  if (!jobInfo) {
    console.error(`No job found for id ${result.job_id}`);
    return;
  }

  if (!jobInfo.userId) {
    console.error(`Job ${jobInfo.jobId} is missing a linked image/user; cannot persist produced image`);
    return;
  }

  const errorMessage = result.error_message ?? result.error ?? "Unknown error";

  await upsertJobStats(jobInfo.jobId, {
    processingTimeMs: Math.round(result.processing_time_ms),
    lastError: result.success ? null : errorMessage,
    finishedAt: new Date()
  });

  if (result.success && result.output_path) {
    try {
      await processSuccessfulResult(jobInfo, result.output_path);
    } catch (error) {
      console.error("Failed to process successful result:", error);
      await updateJobStatus(jobInfo.jobId, "failed");
    }
  } else {
    await updateJobStatus(jobInfo.jobId, "failed");
  }

  await checkRunCompletion(jobInfo.runId);
};

const createJobResultProcessor = () => {
  type Consumer = ReturnType<typeof createQueueConsumer<SkeletonizationWorkerResult>>;
  let consumer: Consumer | null = null;
  let running = false;

  return {
    start: async () => {
      if (running) {
        console.log("Job result processor is already running");
        return;
      }

      running = true;
      consumer = createQueueConsumer<SkeletonizationWorkerResult>(QUEUE_NAMES.SKELETONIZATION_RESULTS);

      console.log("Starting job result processor...");

      consumer
        .consume(async (result) => {
          console.log(`Processing result for job ${result.job_id}, algorithm ${result.algorithm}`);
          await processResult(result);
        })
        .catch((error) => {
          console.error("Job result processor crashed:", error);
          running = false;
        });
    },

    stop: () => {
      consumer?.stop();
      consumer = null;
      running = false;
      console.log("Job result processor stopped");
    },

    get isRunning() {
      return running;
    }
  };
};

export const jobResultProcessor = createJobResultProcessor();
