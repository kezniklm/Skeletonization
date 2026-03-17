/**
 * @file queue.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Redis queue publisher and consumer helpers.
 * @description Provides typed queue abstractions for batch publishing and blocking-pop consumption loops.
 * @version 1.0
 * @date 2026-03-16
 */

import { redisClientManager } from "./client";

/**
 * @brief Interface for publishing queue items.
 */
export type QueuePublisher<T> = {
  publish(items: T[]): Promise<void>;
};

/**
 * @brief Interface for consuming queue items.
 */
export type QueueConsumer<T> = {
  consume(callback: (item: T) => Promise<void>): Promise<void>;
  stop(): void;
};

/** @brief Creates a queue publisher bound to queue name. */
export const createQueuePublisher = <T>(queueName: string): QueuePublisher<T> => ({
  publish: async (items: T[]) => {
    if (items.length === 0) return;

    const client = await redisClientManager.getClient();
    const payloads = items.map((item) => JSON.stringify(item));
    await client.lPush(queueName, payloads);
  }
});

/** @brief Creates a polling queue consumer bound to queue name. */
export const createQueueConsumer = <T>(queueName: string, pollTimeoutSeconds = 5): QueueConsumer<T> => {
  let isRunning = false;

  return {
    consume: async (callback: (item: T) => Promise<void>) => {
      const client = await redisClientManager.getClient();
      isRunning = true;

      while (isRunning) {
        try {
          const result = await client.brPop(queueName, pollTimeoutSeconds);

          if (!result) continue;

          const item = JSON.parse(result.element) as T;
          await callback(item);
        } catch (error) {
          console.error(`Error consuming from queue ${queueName}:`, error);
          await new Promise((resolve) => setTimeout(resolve, 1000));
        }
      }
    },
    stop: () => {
      isRunning = false;
    }
  };
};
