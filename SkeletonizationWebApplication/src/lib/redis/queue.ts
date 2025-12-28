import { redisClientManager } from "./client";

export type QueuePublisher<T> = {
  publish(items: T[]): Promise<void>;
};

export type QueueConsumer<T> = {
  consume(callback: (item: T) => Promise<void>): Promise<void>;
  stop(): void;
};

export const createQueuePublisher = <T>(queueName: string): QueuePublisher<T> => ({
  publish: async (items: T[]) => {
    if (items.length === 0) return;

    const client = await redisClientManager.getClient();
    const payloads = items.map((item) => JSON.stringify(item));
    await client.lPush(queueName, payloads);
  }
});

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
