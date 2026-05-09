/**
 * @file small-screen-overlay.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Renders a fullscreen notice for undersized displays.
 * @description Guards the experience when the screen is smaller than the recommended minimum.
 * @version 1.0
 * @date 2026-05-09
 */

"use client";

import { useEffect, useState } from "react";

const MIN_VIEWPORT_WIDTH = 1366;
const MIN_VIEWPORT_HEIGHT = 768;

const isViewportTooSmall = () => {
  if (typeof window === "undefined") {
    return false;
  }

  const viewport = window.visualViewport;
  const width = viewport?.width ?? window.innerWidth;
  const height = viewport?.height ?? window.innerHeight;

  return width < MIN_VIEWPORT_WIDTH || height < MIN_VIEWPORT_HEIGHT;
};

/**
 * @brief Shows a fullscreen overlay when the detected screen is too small.
 * @description Uses physical screen resolution as a proxy for device size.
 * @returns Null when the screen is large enough; otherwise a blocking overlay.
 */
export const SmallScreenOverlay = () => {
  const [isTooSmall, setIsTooSmall] = useState(false);

  useEffect(() => {
    const evaluate = () => setIsTooSmall(isViewportTooSmall());

    evaluate();
    window.addEventListener("resize", evaluate);
    window.addEventListener("orientationchange", evaluate);
    window.visualViewport?.addEventListener("resize", evaluate);

    return () => {
      window.removeEventListener("resize", evaluate);
      window.removeEventListener("orientationchange", evaluate);
      window.visualViewport?.removeEventListener("resize", evaluate);
    };
  }, []);

  useEffect(() => {
    document.body.style.overflow = isTooSmall ? "hidden" : "";

    return () => {
      document.body.style.overflow = "";
    };
  }, [isTooSmall]);

  if (!isTooSmall) {
    return null;
  }

  return (
    <div
      className="small-screen-overlay"
      role="dialog"
      aria-modal="true"
      aria-labelledby="small-screen-title"
      aria-describedby="small-screen-description"
    >
      <div className="small-screen-overlay__frame">
        <p className="small-screen-overlay__eyebrow">Screen too small</p>
        <h1 id="small-screen-title" className="small-screen-overlay__title">
          Use a larger display
        </h1>
        <p id="small-screen-description" className="small-screen-overlay__copy">
          This experience is optimized for screens 14 inches and larger. Please switch to a larger device to continue.
        </p>
        <div className="small-screen-overlay__meta">Minimum recommended: 1366 x 768.</div>
      </div>
    </div>
  );
};
