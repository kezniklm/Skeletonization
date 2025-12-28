import { createQueuePublisher, QUEUE_NAMES, type SkeletonizationJob } from "./redis/index";

export { type SkeletonizationJob } from "./redis/index";

const jobPublisher = createQueuePublisher<SkeletonizationJob>(QUEUE_NAMES.SKELETONIZATION_JOBS);

export const publishJobs = (jobs: SkeletonizationJob[]): Promise<void> => jobPublisher.publish(jobs);
