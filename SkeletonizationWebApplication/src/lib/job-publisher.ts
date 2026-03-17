/**
 * @file job-publisher.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Publishes skeletonization jobs to Redis queue.
 * @description Exposes typed job publisher utility for enqueuing worker tasks.
 * @version 1.0
 * @date 2026-03-16
 */

import { createQueuePublisher, QUEUE_NAMES, type SkeletonizationJob } from "./redis/index";

export { type SkeletonizationJob } from "./redis/index";

const jobPublisher = createQueuePublisher<SkeletonizationJob>(QUEUE_NAMES.SKELETONIZATION_JOBS);

/** @brief Publishes a batch of skeletonization jobs. */
export const publishJobs = (jobs: SkeletonizationJob[]): Promise<void> => jobPublisher.publish(jobs);
