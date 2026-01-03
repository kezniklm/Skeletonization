import { redisClientManager } from "./client";

type Callback<T> = (event: T) => void;

export type PubSubChannel<T> = {
  publish(event: T): Promise<void>;
  subscribe(callback: Callback<T>): () => void;
};

export const createPubSubChannel = <T>(channelName: string): PubSubChannel<T> => {
  const callbacks = new Set<Callback<T>>();
  let isSubscribed = false;
  let subscriberClient: Awaited<ReturnType<typeof redisClientManager.createSubscriberClient>> | null = null;

  const ensureSubscribed = async () => {
    if (isSubscribed) return;

    isSubscribed = true;
    subscriberClient = await redisClientManager.createSubscriberClient();

    await subscriberClient.subscribe(channelName, (message) => {
      try {
        const event = JSON.parse(message) as T;
        for (const cb of callbacks) {
          try {
            cb(event);
          } catch (err) {
            console.error(`Error in pubsub callback for ${channelName}:`, err);
          }
        }
      } catch (err) {
        console.error(`Error parsing pubsub message on ${channelName}:`, err);
      }
    });

    console.log(`Redis PubSub: Subscribed to ${channelName}`);
  };

  const unsubscribeIfEmpty = async () => {
    if (callbacks.size === 0 && isSubscribed && subscriberClient) {
      try {
        await subscriberClient.unsubscribe(channelName);
        console.log(`Redis PubSub: Unsubscribed from ${channelName}`);
        await subscriberClient.quit();
      } catch (err) {
        console.error(`Error cleaning up subscriber for ${channelName}:`, err);
      } finally {
        subscriberClient = null;
        isSubscribed = false;
      }
    }
  };

  return {
    publish: async (event: T) => {
      const client = await redisClientManager.getClient();
      await client.publish(channelName, JSON.stringify(event));
    },
    subscribe: (callback: Callback<T>) => {
      callbacks.add(callback);
      ensureSubscribed().catch(console.error);

      return () => {
        callbacks.delete(callback);
        unsubscribeIfEmpty().catch(console.error);
      };
    }
  };
};
