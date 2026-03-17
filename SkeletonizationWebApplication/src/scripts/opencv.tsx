/**
 * @file opencv.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Loads OpenCV.js script into the application.
 * @description Exposes a script component for lazy-loading OpenCV runtime in client pages.
 * @version 1.0
 * @date 2026-03-16
 */

import Script from "next/script";

/** @brief Renders OpenCV.js lazy-load script tag. */
export const OpenCvScript = () => (
  <Script id="opencv-js" src="https://docs.opencv.org/4.12.0/opencv.js" strategy="lazyOnload" />
);
