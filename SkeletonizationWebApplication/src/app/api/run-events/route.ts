import { headers } from "next/headers";

import { auth } from "@/auth";
import { subscribeToRunEvents } from "@/lib/run-events";

export const dynamic = "force-dynamic";

export const GET = async (request: Request) => {
  const session = await auth.api.getSession({ headers: await headers() });

  if (!session?.user?.id) {
    return new Response("Unauthorized", { status: 401 });
  }

  const userId = session.user.id;
  const encoder = new TextEncoder();

  let heartbeatInterval: ReturnType<typeof setInterval> | null = null;
  let unsubscribe: (() => void) | null = null;

  const abortHandler = () => {
    cleanup();
  };

  const cleanup = () => {
    if (heartbeatInterval) {
      clearInterval(heartbeatInterval);
      heartbeatInterval = null;
    }
    if (unsubscribe) {
      unsubscribe();
      unsubscribe = null;
    }
    request.signal.removeEventListener("abort", abortHandler);
  };

  request.signal.addEventListener("abort", abortHandler);

  const stream = new ReadableStream({
    start: (controller) => {
      const sendEvent = (event: string, data: unknown) => {
        if (request.signal.aborted) return;

        try {
          const payload = `event: ${event}\ndata: ${JSON.stringify(data)}\n\n`;
          controller.enqueue(encoder.encode(payload));
        } catch {
          cleanup();
        }
      };

      heartbeatInterval = setInterval(() => {
        sendEvent("heartbeat", { ts: Date.now() });
      }, 30_000);

      unsubscribe = subscribeToRunEvents(userId, (event) => {
        sendEvent("run-completed", event);
      });

      sendEvent("connected", { userId });
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
