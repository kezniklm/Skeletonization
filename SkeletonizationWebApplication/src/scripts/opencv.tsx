import Script from "next/script";

export const OpenCvScript = () => (
  <Script id="opencv-js" src="https://docs.opencv.org/4.12.0/opencv.js" strategy="lazyOnload" />
);
