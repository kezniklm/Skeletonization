/**
 * @file use-image-loader.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Loads image resources for preprocessing.
 * @description Tracks asynchronous image loading lifecycle and returns normalized status and error information.
 */

"use client";

import { useEffect, useState } from "react";

type ImageStatus = "idle" | "loading" | "ready" | "error";

type LoaderState = {
  url: string | null;
  image: HTMLImageElement | null;
  error: boolean;
};

/**
 * @brief Loads an image URL and exposes loading state.
 * @description Creates an `Image` object for the provided URL and reports `loading`, `ready`, or `error` status with payload data.
 * @param url Source URL of the image to load.
 * @returns Object containing image loading status, loaded image element, and optional error message.
 */
export const useImageLoader = (url: string | null) => {
  const [state, setState] = useState<LoaderState>({
    url: null,
    image: null,
    error: false
  });

  useEffect(() => {
    if (!url) return;

    let cancelled = false;

    const img = new Image();
    img.crossOrigin = "anonymous";
    img.src = url;

    img.onload = () => {
      if (cancelled) return;
      setState({
        url,
        image: img,
        error: false
      });
    };

    img.onerror = () => {
      if (cancelled) return;
      setState({
        url,
        image: null,
        error: true
      });
    };

    return () => {
      cancelled = true;
    };
  }, [url]);

  let status: ImageStatus;
  let image: HTMLImageElement | null = null;

  if (!url) {
    status = "error";
  } else if (state.url === url) {
    if (state.error) {
      status = "error";
    } else if (state.image) {
      status = "ready";
      image = state.image;
    } else {
      status = "loading";
    }
  } else {
    status = "loading";
  }

  return { status, image, errorMessage: state.error ? "Failed to load image" : null };
};
