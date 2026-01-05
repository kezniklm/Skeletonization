import { z } from "zod";

import { type Algorithm } from "@/algorithms";
import { type FileOutputFormat } from "@/database/zod";
import { selectJobStatsSchema, type WorkerType } from "@/database/zod/job-stats";

export const QUEUE_NAMES = {
  SKELETONIZATION_JOBS: process.env.JOBS_QUEUE ?? "skeletonization:jobs",
  SKELETONIZATION_RESULTS: process.env.RESULTS_QUEUE ?? "skeletonization:results"
} as const;

export type SkeletonizationJob = {
  id: string;
  tasks: Array<{
    image_key: string;
    algorithm: Algorithm;
    should_run_preprocessing: boolean;
    output_format: FileOutputFormat;
  }>;
};

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
