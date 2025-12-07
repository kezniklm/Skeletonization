"use client";

import { type CV } from "mirada/dist/src/types/opencv";
import { useEffect, useState } from "react";

type OpenCvStatus = "loading" | "ready" | "error";

type UseOpenCvOptions = {
  timeoutMs?: number;
};

type CvWithRuntime = CV & {
  onRuntimeInitialized?: () => unknown;
};

type WindowWithCv = Window & {
  cv?: CvWithRuntime;
};

type UseOpenCvResultLoading = {
  status: "loading";
  isLoaded: false;
  error: null;
  cv: null;
};

type UseOpenCvResultError = {
  status: "error";
  isLoaded: false;
  error: string;
  cv: null;
};

type UseOpenCvResultReady = {
  status: "ready";
  isLoaded: true;
  error: null;
  cv: CV;
};

export type UseOpenCvResult = UseOpenCvResultLoading | UseOpenCvResultError | UseOpenCvResultReady;

const getWindowWithCv = (): WindowWithCv | null => {
  if (typeof window === "undefined") return null;
  return window as WindowWithCv;
};

const getCv = (): CV | null => {
  const win = getWindowWithCv();
  return win?.cv ?? null;
};

export const useOpenCV = (options?: UseOpenCvOptions): UseOpenCvResult => {
  const timeoutMs = options?.timeoutMs ?? 0;

  const [status, setStatus] = useState<OpenCvStatus>(() => {
    const cv = getCv();
    return cv?.Mat ? "ready" : "loading";
  });

  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    const win = getWindowWithCv();
    if (!win) return;

    let cancelled = false;
    let intervalId: number | undefined;
    let timeoutId: number | undefined;

    const startTimeoutIfNeeded = () => {
      if (!timeoutMs) return;

      timeoutId = win.setTimeout(() => {
        if (cancelled) return;

        setStatus((prev) => (prev === "ready" ? prev : "error"));
        setError((prev) => prev ?? "Timed out while waiting for OpenCV.js to load.");
      }, timeoutMs);
    };

    const clearTimeoutIfAny = () => {
      if (timeoutId !== undefined) {
        win.clearTimeout(timeoutId);
        timeoutId = undefined;
      }
    };

    const cv = win.cv as CvWithRuntime | undefined;

    if (!cv) {
      startTimeoutIfNeeded();

      intervalId = win.setInterval(() => {
        if (cancelled) return;

        const cvNow = win.cv as CvWithRuntime | undefined;
        if (cvNow?.Mat) {
          win.clearInterval(intervalId);
          clearTimeoutIfAny();
          setStatus("ready");
          setError(null);
        }
      }, 100);

      return () => {
        cancelled = true;
        if (intervalId !== undefined) {
          win.clearInterval(intervalId);
        }
        clearTimeoutIfAny();
      };
    }

    if (!cv.Mat) {
      startTimeoutIfNeeded();

      const prevOnRuntimeInitialized = cv.onRuntimeInitialized;

      cv.onRuntimeInitialized = () => {
        if (cancelled) return;

        prevOnRuntimeInitialized?.();

        clearTimeoutIfAny();
        setStatus("ready");
        setError(null);
      };

      return () => {
        cancelled = true;
        cv.onRuntimeInitialized = prevOnRuntimeInitialized;
        clearTimeoutIfAny();
      };
    }

    return () => {
      cancelled = true;
    };
  }, [timeoutMs]);

  if (status === "ready" && !error) {
    const cv = getCv();
    if (!cv) {
      return {
        status: "error",
        isLoaded: false,
        error: "OpenCV reported ready, but cv is missing.",
        cv: null
      };
    }

    return {
      status: "ready",
      isLoaded: true,
      error: null,
      cv
    };
  }

  if (status === "error") {
    return {
      status: "error",
      isLoaded: false,
      error: error ?? "Unknown error while loading OpenCV.js.",
      cv: null
    };
  }

  return {
    status: "loading",
    isLoaded: false,
    error: null,
    cv: null
  };
};
