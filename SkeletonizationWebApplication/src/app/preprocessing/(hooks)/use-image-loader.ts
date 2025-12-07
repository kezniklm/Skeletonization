"use client";

import { useEffect, useState } from "react";

type ImageStatus = "idle" | "loading" | "ready" | "error";

type LoaderState = {
  url: string | null;
  image: HTMLImageElement | null;
  error: boolean;
};

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
