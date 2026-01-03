import { type Algorithm } from "@/algorithms";

export const QUEUE_NAMES = {
  SKELETONIZATION_JOBS: "skeletonization:jobs",
  SKELETONIZATION_RESULTS: "skeletonization:results"
} as const;

export type SkeletonizationJob = {
  id: string;
  tasks: Array<{
    image_key: string;
    algorithm: Algorithm;
    should_run_preprocessing: boolean;
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
