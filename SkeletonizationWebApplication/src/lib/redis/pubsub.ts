/**
 * @file pubsub.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Creates typed Redis pub/sub channels with recent-event backup.
 * @description Implements publish/subscribe helpers with local callback management and short-lived event replay list.
 * @version 1.0
 * @date 2026-03-16
 */

import { redisClientManager } from "./client";

type Callback<T> = (event: T) => void;

/**
 * @brief Interface for typed pub/sub channel operations.
 */
export type PubSubChannel<T> = {
  publish(event: T): Promise<void>;
  subscribe(callback: Callback<T>): () => void;
  getRecentEvents(): Promise<T[]>;
};

const EVENT_TTL_SECONDS = 120;
const MAX_RECENT_EVENTS = 50;

const getBackupListKey = (channelName: string): string => `${channelName}:recent`;

/** @brief Creates a typed Redis pub/sub channel helper. */
export const createPubSubChannel = <T>(channelName: string): PubSubChannel<T> => {
  const callbacks = new Set<Callback<T>>();
  let isSubscribed = false;
  let subscriberClient: Awaited<ReturnType<typeof redisClientManager.createSubscriberClient>> | null = null;

  const backupListKey = getBackupListKey(channelName);

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
      const payload = JSON.stringify(event);

      await client.publish(channelName, payload);

      await client.lPush(backupListKey, payload);
      await client.lTrim(backupListKey, 0, MAX_RECENT_EVENTS - 1);
      await client.expire(backupListKey, EVENT_TTL_SECONDS);
    },
    subscribe: (callback: Callback<T>) => {
      callbacks.add(callback);
      ensureSubscribed().catch(console.error);

      return () => {
        callbacks.delete(callback);
        unsubscribeIfEmpty().catch(console.error);
      };
    },
    getRecentEvents: async (): Promise<T[]> => {
      const client = await redisClientManager.getClient();
      const items = await client.lRange(backupListKey, 0, -1);
      const events: T[] = [];
      for (const item of items) {
        try {
          events.push(JSON.parse(item) as T);
        } catch {
          // skip malformed
        }
      }

      return events.reverse();
    }
  };
};
