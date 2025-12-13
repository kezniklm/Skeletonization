"use server";

import { join } from "path";

import type { SelectImage } from "@/database/zod/image";
import type { InsertJob } from "@/database/zod/job";
import { type CreateSkeletonizationRun, createSkeletonizationRunSchema } from "@/database/zod/run";
import type { InsertRunImage } from "@/database/zod/run-image";
import { publishJob, type SkeletonizationJob } from "@/lib/redis";
import { getImagesByIds } from "@/repositories/image";
import { createJobsBulk } from "@/repositories/job";
import { createRun, getRunById, updateRunStatus } from "@/repositories/run";
import { createRunImagesBulk } from "@/repositories/run-image";

import { requireUser } from "./common";

export const createSkeletonizationRunsAction = async (input: CreateSkeletonizationRun) => {
  const user = await requireUser("create skeletonization run");

  const { name, algorithms, imageIds, params } = createSkeletonizationRunSchema.parse(input);

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

  const runImagesData: InsertRunImage[] = [];

  let ordinal = 0;
  for (const imageId of imageIds) {
    for (const algorithm of algorithms) {
      runImagesData.push({
        runId: run.id,
        imageId,
        algorithm,
        ordinal: ordinal++,
        params,
        status: "pending",
        attempt: 0
      });
    }
  }

  const createdRunImages = await createRunImagesBulk(runImagesData);

  const jobsData: InsertJob[] = createdRunImages.map((runImage) => ({
    runImageId: runImage.id,
    status: "queued",
    attempts: 0
  }));

  await createJobsBulk(jobsData);

  const tasks = createdRunImages.map((runImage) => {
    const img = imagesMap.get(runImage.imageId)!;
    const imagePath = join(process.cwd(), "public", img.storagePath);
    return {
      image_path: imagePath,
      algorithm: runImage.algorithm,
      should_run_preprocessing: true
    };
  });

  const redisJob: SkeletonizationJob = { id: run.id, tasks };

  try {
    await publishJob(redisJob);
    await updateRunStatus(run.id, "running", new Date());
  } catch (error) {
    console.error("Failed to publish job to Redis:", error);
    await updateRunStatus(run.id, "failed", new Date());
    throw error;
  }

  return run;
};

export const cancelRunAction = async (runId: string) => {
  const user = await requireUser("cancel runs");

  const run = await getRunById(runId);

  if (!run) {
    throw new Error("Run not found");
  }

  if (run.userId !== user.id) {
    throw new Error("You can only cancel your own runs");
  }

  if (run.status !== "pending") {
    throw new Error("Only pending runs can be cancelled");
  }

  await updateRunStatus(runId, "cancelled", new Date());

  return run;
};
