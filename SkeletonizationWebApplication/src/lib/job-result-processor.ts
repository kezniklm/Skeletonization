import { createImage } from "@/repositories/image";
import { getJobsByRunId, getJobWithOwner, updateJobStatus, type JobWithOwner } from "@/repositories/job";
import { upsertJobStats } from "@/repositories/job-stats";
import { getRunOwnerAndName, getRunStatus, updateRunStatus } from "@/repositories/run";

import { copyOutputFile } from "./output-file-handler";
import { redisClientManager } from "./redis/client";
import { QUEUE_NAMES, skeletonizationWorkerResultSchema, type SkeletonizationWorkerResult } from "./redis/index";
import { publishRunCompletedEvent } from "./run-events";

const checkRunCompletion = async (runId: string): Promise<void> => {
  const status = await getRunStatus(runId);
  if (status === "completed" || status === "failed" || status === "cancelled") return;

  const jobs = await getJobsByRunId(runId);
  const allComplete = jobs.every((j) => j.status === "completed" || j.status === "failed");

  if (!allComplete) return;

  const failedCount = jobs.filter((j) => j.status === "failed").length;
  const anyFailed = failedCount > 0;
  const succeededCount = jobs.filter((j) => j.status === "completed").length;
  const completedAt = new Date();

  const finalStatus = anyFailed ? "failed" : "completed";

  const [runDetails, updatedRun] = await Promise.all([
    getRunOwnerAndName(runId),
    updateRunStatus(runId, finalStatus, completedAt)
  ]);

  // Another concurrent worker already finalized this run.
  if (!updatedRun) {
    return;
  }

  console.log(`Run ${runId} finished (status=${finalStatus}, failed=${failedCount}, succeeded=${succeededCount})`);

  if (runDetails?.userId) {
    await publishRunCompletedEvent({
      runId,
      userId: runDetails.userId,
      runName: runDetails.name,
      status: finalStatus,
      jobCount: jobs.length,
      failedCount,
      succeededCount,
      completedAt: updatedRun.completedAt?.toISOString() ?? completedAt.toISOString()
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

export const processResult = async (result: SkeletonizationWorkerResult): Promise<void> => {
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
    workerId: result.worker_id,
    workerType: result.worker_type,
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

export type BatchProcessResult = {
  processed: number;
  errors: number;
  duration: number;
  stoppedEarly: boolean;
};

export const processBatch = async (maxBatchSize = 20, maxExecutionTimeMs = 10000): Promise<BatchProcessResult> => {
  const startTime = Date.now();
  let processed = 0;
  let errors = 0;

  const client = await redisClientManager.getClient();
  const queueName = QUEUE_NAMES.SKELETONIZATION_RESULTS;

  console.log(`[Batch] Starting batch processing from queue: ${queueName}`);

  while (processed < maxBatchSize) {
    if (Date.now() - startTime > maxExecutionTimeMs) {
      console.log(`[Batch] Approaching time limit, stopping after ${processed} items`);
      break;
    }

    const item = await client.rPop(queueName);

    if (!item) {
      console.log(`[Batch] Queue empty after processing ${processed} items`);
      break;
    }

    try {
      const result = skeletonizationWorkerResultSchema.parse(JSON.parse(item)) as SkeletonizationWorkerResult;
      console.log(`[Batch] Processing result for job ${result.job_id}, algorithm ${result.algorithm}`);
      await processResult(result);
      processed++;
    } catch (error) {
      errors++;
      console.error(`[Batch] Error processing result:`, error);
    }
  }

  const duration = Date.now() - startTime;
  console.log(`[Batch] Completed: ${processed} processed, ${errors} errors, ${duration}ms`);

  return {
    processed,
    errors,
    duration,
    stoppedEarly: processed >= maxBatchSize || Date.now() - startTime > maxExecutionTimeMs
  };
};
