/**
 * @file index.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Re-exports Redis client, queue, pubsub, and types modules.
 * @description Provides centralized Redis integration exports for publishers, consumers, channels, and schemas.
 * @version 1.0
 * @date 2026-03-16
 */

export { redisClientManager } from "./client";
export { createPubSubChannel, type PubSubChannel } from "./pubsub";
export { createQueueConsumer, createQueuePublisher, type QueueConsumer, type QueuePublisher } from "./queue";
export {
  QUEUE_NAMES,
  skeletonizationWorkerResultSchema,
  type SkeletonizationJob,
  type SkeletonizationWorkerResult
} from "./types";
