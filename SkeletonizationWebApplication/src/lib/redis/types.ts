import { type Algorithm } from "@/algorithms";
import { type FileOutputFormat } from "@/database/zod";

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
    outputFormat: FileOutputFormat;
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
};
