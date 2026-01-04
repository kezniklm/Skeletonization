import { createPubSubChannel } from "./redis/index";

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

export const subscribeToRunEvents = (userId: string, callback: (event: RunCompletedEvent) => void): (() => void) => {
  const channel = getUserChannel(userId);

  return channel.subscribe((event) => {
    callback(event);
  });
};

export const publishRunCompletedEvent = async (event: RunCompletedEvent): Promise<void> => {
  const channel = getUserChannel(event.userId);
  await channel.publish(event);
  console.log(`Published run-completed event for run ${event.runId} to user ${event.userId}`);
};

export const getRecentRunEvents = async (userId: string): Promise<RunCompletedEvent[]> => {
  const channel = getUserChannel(userId);
  return channel.getRecentEvents();
};
