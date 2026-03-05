import { useEffect, useRef } from "react";

import { drawImageElementToCanvas } from "../utils/canvas";

export const usePreprocessingCanvas = (originalImage: HTMLImageElement) => {
  const originalImageRef = useRef<HTMLImageElement>(originalImage);

  const canvasRef = useRef<HTMLCanvasElement>(null!);
  const comparisonCanvasRef = useRef<HTMLCanvasElement>(null!);
  const baseCanvasRef = useRef<HTMLCanvasElement | null>(null);

  useEffect(() => {
    originalImageRef.current = originalImage;

    baseCanvasRef.current ??= document.createElement("canvas");

    drawImageElementToCanvas(baseCanvasRef.current, originalImage);
  }, [originalImage]);

  return {
    originalImageRef,
    canvasRef,
    comparisonCanvasRef,
    baseCanvasRef
  };
};
