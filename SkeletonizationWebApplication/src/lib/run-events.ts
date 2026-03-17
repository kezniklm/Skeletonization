/**
 * @file run-events.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Publishes and subscribes user-scoped run completion events.
 * @description Manages per-user pub/sub channels for run completion notifications and recent-event retrieval.
 * @version 1.0
 * @date 2026-03-16
 */

import { createPubSubChannel } from "./redis/index";

/**
 * @brief Payload structure for run completion notifications.
 */
export type RunCompletedEvent = {
  runId: string;
  userId: string;
  runName: string | null;
  status: "completed" | "failed";
  jobCount: number;
  failedCount: number;
  succeededCount: number;
  completedAt: string;
};

const CHANNEL_PREFIX = "run-events:";

const userChannels = new Map<string, ReturnType<typeof createPubSubChannel<RunCompletedEvent>>>();

const getUserChannel = (userId: string) => {
  let channel = userChannels.get(userId);
  if (!channel) {
    channel = createPubSubChannel<RunCompletedEvent>(`${CHANNEL_PREFIX}${userId}`);
    userChannels.set(userId, channel);
  }
  return channel;
};

/** @brief Subscribes callback to run completion events for user. */
export const subscribeToRunEvents = (userId: string, callback: (event: RunCompletedEvent) => void): (() => void) => {
  const channel = getUserChannel(userId);

  return channel.subscribe((event) => {
    callback(event);
  });
};

/** @brief Publishes run completion event to a user channel. */
export const publishRunCompletedEvent = async (event: RunCompletedEvent): Promise<void> => {
  const channel = getUserChannel(event.userId);
  await channel.publish(event);
  console.log(`Published run-completed event for run ${event.runId} to user ${event.userId}`);
};

/** @brief Returns recent cached run events for a user. */
export const getRecentRunEvents = async (userId: string): Promise<RunCompletedEvent[]> => {
  const channel = getUserChannel(userId);
  return channel.getRecentEvents();
};
