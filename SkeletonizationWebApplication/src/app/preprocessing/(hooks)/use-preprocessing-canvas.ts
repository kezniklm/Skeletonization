import { useEffect, useRef } from "react";

export const usePreprocessingCanvas = (originalImage: HTMLImageElement) => {
  const originalImageRef = useRef<HTMLImageElement>(originalImage);

  const canvasRef = useRef<HTMLCanvasElement>(null!);
  const comparisonCanvasRef = useRef<HTMLCanvasElement>(null!);

  useEffect(() => {
    originalImageRef.current = originalImage;
  }, [originalImage]);

  return {
    originalImageRef,
    canvasRef,
    comparisonCanvasRef
  };
};
