"use server";

import type { SelectImage } from "@/database/zod/image";
import type { InsertJob } from "@/database/zod/job";
import { type CreateSkeletonizationRun, createSkeletonizationRunSchema } from "@/database/zod/run";
import { publishJobs, type SkeletonizationJob } from "@/lib/job-publisher";
import { getImagesByIds } from "@/repositories/image";
import { createJobsBulk } from "@/repositories/job";
import { createRun, updateRunStatus } from "@/repositories/run";

import { requireUser } from "./common";

export const createSkeletonizationRunsAction = async (input: CreateSkeletonizationRun) => {
  const user = await requireUser("create skeletonization run");

  const { name, algorithms, imageIds, params } = createSkeletonizationRunSchema.parse(input);

  const getShouldRunPreprocessing = (imageId: string) =>
    params.preprocessByImageId[imageId] ?? params.preprocessAllImages;

  const images = await getImagesByIds(imageIds);

  const imagesMap = new Map<string, SelectImage>(images.map((img) => [img.id, img]));

  const missingImageIds = imageIds.filter((id) => !imagesMap.has(id));

  if (missingImageIds.length > 0) {
    throw new Error(`Images not found: ${missingImageIds.join(", ")}`);
  }

  const unauthorizedImageIds = images.filter((img) => img.userId !== user.id).map((img) => img.id);
  if (unauthorizedImageIds.length > 0) {
    throw new Error(`You can only process your own images. Unauthorized image ids: ${unauthorizedImageIds.join(", ")}`);
  }

  const run = await createRun({
    userId: user.id,
    name,
    params,
    status: "pending"
  });

  const jobsData: InsertJob[] = [];

  let ordinal = 0;
  for (const imageId of imageIds) {
    for (const algorithm of algorithms) {
      jobsData.push({
        runId: run.id,
        imageId,
        algorithm,
        ordinal: ordinal++,
        params,
        status: "queued"
      });
    }
  }

  const createdJobs = await createJobsBulk(jobsData);

  try {
    const redisJobs: SkeletonizationJob[] = createdJobs.map((job) => {
      const img = imagesMap.get(job.imageId)!;

      return {
        id: job.id,
        tasks: [
          {
            image_key: img.storagePath,
            algorithm: job.algorithm,
            should_run_preprocessing: getShouldRunPreprocessing(job.imageId),
            outputFormat: params.outputFormat
          }
        ]
      };
    });

    await publishJobs(redisJobs);
    await updateRunStatus(run.id, "running", new Date());
  } catch (error) {
    console.error("Failed to publish jobs to Redis:", error);
    await updateRunStatus(run.id, "failed", new Date());
    throw error;
  }

  return run;
};
