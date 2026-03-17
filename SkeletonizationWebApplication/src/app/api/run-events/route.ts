/**
 * @file route.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Implements run-events Server-Sent Events endpoint.
 * @description Streams run completion updates with heartbeat, TTL handling, and recent-event polling for authenticated users.
 * @version 1.0
 * @date 2026-03-16
 */

import { headers } from "next/headers";

import { auth } from "@/auth";
import { getRecentRunEvents, subscribeToRunEvents } from "@/lib/run-events";

/**
 * @brief Forces dynamic execution for SSE endpoint.
 * @description Prevents response caching and static optimization.
 */
export const dynamic = "force-dynamic";

/**
 * @brief Sets maximum execution window for SSE request.
 * @description Limits long-running route execution time in seconds.
 */
export const maxDuration = 60;

/**
 * @brief Defines maximum lifetime of one SSE connection.
 * @description Connection is rotated after this interval to reduce stale subscriptions.
 */
const SSE_CONNECTION_TTL_MS = 50_000;

/**
 * @brief Defines polling interval for missed recent run events.
 * @description Supplements pub/sub delivery to improve reliability.
 */
const RECENT_EVENTS_POLL_MS = 2000;

/**
 * @brief Opens authenticated SSE stream of run events.
 * @param request Incoming HTTP request with abort signal.
 * @returns Streaming text/event-stream response or unauthorized response.
 */
export const GET = async (request: Request) => {
  const session = await auth.api.getSession({ headers: await headers() });

  if (!session?.user?.id) {
    return new Response("Unauthorized", { status: 401 });
  }

  const userId = session.user.id;
  const encoder = new TextEncoder();

  let heartbeatInterval: ReturnType<typeof setInterval> | null = null;
  let unsubscribe: (() => void) | null = null;
  let ttlTimeout: ReturnType<typeof setTimeout> | null = null;
  let recentPollInterval: ReturnType<typeof setInterval> | null = null;

  const abortHandler = () => {
    cleanup();
  };

  /**
   * @brief Releases timers, subscriptions, and abort listeners.
   * @returns No return value.
   */
  const cleanup = () => {
    if (heartbeatInterval) {
      clearInterval(heartbeatInterval);
      heartbeatInterval = null;
    }
    if (ttlTimeout) {
      clearTimeout(ttlTimeout);
      ttlTimeout = null;
    }
    if (recentPollInterval) {
      clearInterval(recentPollInterval);
      recentPollInterval = null;
    }
    if (unsubscribe) {
      unsubscribe();
      unsubscribe = null;
    }
    request.signal.removeEventListener("abort", abortHandler);
  };

  request.signal.addEventListener("abort", abortHandler);

  const sentEventKeys = new Set<string>();

  const stream = new ReadableStream({
    start: async (controller) => {
      const sendEvent = (event: string, data: unknown) => {
        if (request.signal.aborted) return;

        try {
          const payload = `event: ${event}\ndata: ${JSON.stringify(data)}\n\n`;
          controller.enqueue(encoder.encode(payload));
        } catch {
          cleanup();
        }
      };

      const sendRunCompleted = (event: { runId: string; completedAt?: string }) => {
        const key = `${event.runId}:${event.completedAt ?? ""}`;
        if (sentEventKeys.has(key)) return;
        sentEventKeys.add(key);
        sendEvent("run-completed", event);
      };

      ttlTimeout = setTimeout(() => {
        try {
          sendEvent("server-restart", { reason: "ttl", ts: Date.now() });
          cleanup();
          controller.close();
        } catch {
          cleanup();
        }
      }, SSE_CONNECTION_TTL_MS);

      heartbeatInterval = setInterval(() => {
        sendEvent("heartbeat", { ts: Date.now() });
      }, 30_000);

      unsubscribe = subscribeToRunEvents(userId, (event) => {
        sendRunCompleted(event);
      });

      sendEvent("connected", { userId });

      const pollRecentEvents = async () => {
        try {
          const recentEvents = await getRecentRunEvents(userId);
          for (const event of recentEvents) {
            sendRunCompleted(event);
          }
        } catch (err) {
          console.error("Failed to poll recent run events:", err);
        }
      };

      recentPollInterval = setInterval(() => {
        void pollRecentEvents();
      }, RECENT_EVENTS_POLL_MS);
    },
    cancel: () => cleanup()
  });

  return new Response(stream, {
    headers: {
      "Content-Type": "text/event-stream",
      "Cache-Control": "no-cache, no-transform",
      Connection: "keep-alive"
    }
  });
};
