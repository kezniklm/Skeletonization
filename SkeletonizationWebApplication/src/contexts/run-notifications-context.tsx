"use client";

import { type PropsWithChildren, useEffect, useRef } from "react";
import { usePathname, useRouter } from "next/navigation";
import { toast } from "sonner";

import { showSkeletonizationCompleteNotification } from "@/lib/notifications";

type RunCompletedEvent = {
  runId: string;
  runName: string | null;
  status: "completed" | "failed";
  jobCount: number;
  failedCount: number;
  succeededCount: number;
  completedAt: string;
};

const SEEN_RUNS_STORAGE_KEY = "skeletonization:seen-run-completions";
const MAX_SEEN_RUNS = 300;

const readSeenRunKeys = (): Set<string> => {
  if (typeof window === "undefined") return new Set();

  try {
    const raw = window.sessionStorage.getItem(SEEN_RUNS_STORAGE_KEY);
    if (!raw) return new Set();

    const parsed = JSON.parse(raw) as unknown;
    if (!Array.isArray(parsed)) return new Set();

    const keys = parsed.filter((item): item is string => typeof item === "string");
    return new Set(keys);
  } catch {
    return new Set();
  }
};

const writeSeenRunKeys = (keys: Set<string>) => {
  if (typeof window === "undefined") return;

  try {
    const sliced = Array.from(keys).slice(-MAX_SEEN_RUNS);
    window.sessionStorage.setItem(SEEN_RUNS_STORAGE_KEY, JSON.stringify(sliced));
  } catch {
    // best effort only
  }
};

export const RunNotificationsProvider = ({ children, enabled }: PropsWithChildren<{ enabled: boolean }>) => {
  const eventSourceRef = useRef<EventSource | null>(null);
  const processedRunIdsRef = useRef<Set<string>>(readSeenRunKeys());
  const router = useRouter();
  const pathname = usePathname();

  useEffect(() => {
    if (!enabled) return;

    const eventSource = new EventSource("/api/run-events");
    eventSourceRef.current = eventSource;

    eventSource.addEventListener("open", () => {
      console.log("SSE: Connected to run events");
    });

    eventSource.addEventListener("run-completed", (event) => {
      try {
        const data = JSON.parse(event.data) as RunCompletedEvent;

        if (processedRunIdsRef.current.has(data.runId)) {
          return;
        }

        processedRunIdsRef.current.add(data.runId);
        writeSeenRunKeys(processedRunIdsRef.current);

        const trimmedName = data.runName?.trim();
        const displayName = trimmedName ?? `Run ${data.runId.slice(0, 8)}`;

        console.log("SSE: Received run-completed event", data);

        if (pathname.startsWith("/lab")) {
          router.refresh();
        }

        if ("Notification" in window && Notification.permission === "granted") {
          showSkeletonizationCompleteNotification(displayName, data.jobCount);
        }

        if (data.status === "failed") {
          toast.error(`Run "${displayName}" finished with errors`, {
            description: `${data.succeededCount} succeeded, ${data.failedCount} failed`
          });
        } else {
          toast.success(`Run "${displayName}" completed`, {
            description: `${data.jobCount} image${data.jobCount !== 1 ? "s" : ""} processed successfully`
          });
        }
      } catch (err) {
        console.error("Error parsing run-completed event:", err);
      }
    });

    eventSource.addEventListener("heartbeat", () => {});

    eventSource.addEventListener("error", () => {});

    return () => {
      console.log("SSE: Closing connection");
      eventSource.close();
      eventSourceRef.current = null;
    };
  }, [enabled, pathname, router]);

  return children;
};
