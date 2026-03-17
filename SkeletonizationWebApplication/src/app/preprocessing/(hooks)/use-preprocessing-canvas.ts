/**
 * @file use-preprocessing-canvas.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Initializes canvas references used by preprocessing workspace.
 * @description Creates and maintains refs for output, comparison, and base canvases synchronized with the selected source image.
 */

import { useEffect, useRef } from "react";

import { drawImageElementToCanvas } from "../utils/canvas";

/**
 * @brief Prepares canvas references and base image state for preprocessing.
 * @description Sets up persistent refs for original image and canvases, and draws the original image into the processing base canvas.
 * @param originalImage Loaded source image element.
 * @returns Refs for original image and workspace canvases.
 */
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
