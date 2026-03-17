/**
 * @file types.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Defines Redis queue names and payload schemas.
 * @description Provides typed structures and validation schemas for job queue and worker result messages.
 * @version 1.0
 * @date 2026-03-16
 */

import { z } from "zod";

import { type Algorithm } from "@/algorithms";
import { type FileOutputFormat } from "@/database/zod";
import { selectJobStatsSchema, type WorkerType } from "@/database/zod/job-stats";

/** @brief Redis queue names used by processing pipeline. */
export const QUEUE_NAMES = {
  SKELETONIZATION_JOBS: process.env.JOBS_QUEUE ?? "skeletonization:jobs",
  SKELETONIZATION_RESULTS: process.env.RESULTS_QUEUE ?? "skeletonization:results"
} as const;

/**
 * @brief Payload structure for skeletonization job queue items.
 */
export type SkeletonizationJob = {
  id: string;
  tasks: Array<{
    image_key: string;
    algorithm: Algorithm;
    should_run_preprocessing: boolean;
    output_format: FileOutputFormat;
  }>;
};

/**
 * @brief Payload structure for worker result queue items.
 */
export type SkeletonizationWorkerResult = {
  job_id: string;
  image_path: string;
  output_path: string;
  success: boolean;
  processing_time_ms: number;
  error_message?: string;
  error?: string;
  algorithm: string;
  worker_id: string;
  worker_type: WorkerType;
};

/** @brief Zod schema for validating worker result payloads. */
export const skeletonizationWorkerResultSchema = z.object({
  job_id: z.string(),
  image_path: z.string(),
  output_path: z.string(),
  success: z.boolean(),
  processing_time_ms: z.number(),
  error_message: z.string().optional(),
  error: z.string().optional(),
  algorithm: z.string(),
  worker_id: selectJobStatsSchema.shape.workerId,
  worker_type: selectJobStatsSchema.shape.workerType
});
