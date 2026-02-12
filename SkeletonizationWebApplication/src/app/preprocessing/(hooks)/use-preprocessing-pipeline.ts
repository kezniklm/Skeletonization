"use client";

import { type CV, type Mat } from "mirada/dist/src/types/opencv";
import { type RefObject, useEffect } from "react";

import type { FilterState, TransformState } from "../types";
import { drawImageElementToCanvas, get2DContextOrThrow } from "../utils/canvas";

type Args = {
  cv: CV;
  filters: FilterState;
  transforms: TransformState;
  originalImageRef: RefObject<HTMLImageElement | null>;
  canvasRef: RefObject<HTMLCanvasElement | null>;
  comparisonCanvasRef: RefObject<HTMLCanvasElement | null>;
  showComparison: boolean;
  setProcessing: React.Dispatch<React.SetStateAction<boolean>>;
  skipNextProcessingRef?: RefObject<boolean>;
};

type ProcessingParams = {
  cv: CV;
  filters: FilterState;
  transforms: TransformState;
  img: HTMLImageElement;
  canvas: HTMLCanvasElement;
  setProcessing: React.Dispatch<React.SetStateAction<boolean>>;
};

const applyGrayscale = (cv: CV, filters: FilterState, dst: Mat) => {
  if (!filters.grayscale) return;
  cv.cvtColor(dst, dst, cv.COLOR_RGBA2GRAY);
  cv.cvtColor(dst, dst, cv.COLOR_GRAY2RGBA);
};

const applyDenoise = (cv: CV, filters: FilterState, dst: Mat) => {
  if (filters.denoise <= 0) return;

  const rgb = new cv.Mat();
  const temp = new cv.Mat();
  try {
    cv.cvtColor(dst, rgb, cv.COLOR_RGBA2RGB);
    const sigmaColor = filters.denoise * 10;
    const sigmaSpace = filters.denoise * 10;
    cv.bilateralFilter(rgb, temp, 9, sigmaColor, sigmaSpace, cv.BORDER_DEFAULT);
    cv.cvtColor(temp, dst, cv.COLOR_RGB2RGBA);
  } finally {
    rgb.delete();
    temp.delete();
  }
};

const applyBlur = (cv: CV, filters: FilterState, dst: Mat) => {
  if (filters.blur <= 0) return;
  const ksize = filters.blur * 2 + 1;
  cv.GaussianBlur(dst, dst, new cv.Size(ksize, ksize), 0);
};

const applyBrightnessContrast = (filters: FilterState, dst: Mat) => {
  if (filters.brightness === 0 && filters.contrast === 1) return;
  dst.convertTo(dst, -1, filters.contrast, filters.brightness);
};

const applySaturationAndHue = (cv: CV, filters: FilterState, dst: Mat) => {
  if (filters.saturation === 1 && filters.hue === 0) return;

  const hsv = new cv.Mat();
  const channels = new cv.MatVector();

  try {
    cv.cvtColor(dst, hsv, cv.COLOR_RGBA2RGB);
    cv.cvtColor(hsv, hsv, cv.COLOR_RGB2HSV);

    cv.split(hsv, channels);

    if (filters.hue !== 0) {
      const hChannel = channels.get(0);
      try {
        hChannel.convertTo(hChannel, -1, 1, filters.hue);
        channels.set(0, hChannel);
      } finally {
        hChannel.delete();
      }
    }

    if (filters.saturation !== 1) {
      const sChannel = channels.get(1);
      try {
        sChannel.convertTo(sChannel, -1, filters.saturation, 0);
        channels.set(1, sChannel);
      } finally {
        sChannel.delete();
      }
    }

    cv.merge(channels, hsv);
    cv.cvtColor(hsv, dst, cv.COLOR_HSV2RGB);
    cv.cvtColor(dst, dst, cv.COLOR_RGB2RGBA);
  } finally {
    hsv.delete();
    channels.delete();
  }
};

const applyThreshold = (cv: CV, filters: FilterState, dst: Mat) => {
  if (!filters.thresholding) {
    return;
  }

  cv.cvtColor(dst, dst, cv.COLOR_RGBA2GRAY);
  const thresholdType = filters.threshold <= 0 ? cv.THRESH_BINARY | cv.THRESH_OTSU : cv.THRESH_BINARY;
  const thresholdValue = filters.threshold <= 0 ? 0 : filters.threshold;
  cv.threshold(dst, dst, thresholdValue, 255, thresholdType);
  cv.cvtColor(dst, dst, cv.COLOR_GRAY2RGBA);
};

const applyEdgeDetection = (cv: CV, filters: FilterState, dst: Mat) => {
  if (!filters.edgeDetection) return;

  const gray = new cv.Mat();
  const edges = new cv.Mat();
  try {
    cv.cvtColor(dst, gray, cv.COLOR_RGBA2GRAY);
    cv.Canny(gray, edges, 50, 150);
    cv.cvtColor(edges, dst, cv.COLOR_GRAY2RGBA);
  } finally {
    gray.delete();
    edges.delete();
  }
};

const applySobelEdgeDetection = (cv: CV, filters: FilterState, dst: Mat) => {
  if (!filters.sobelEdgeDetection) return;

  const gray = new cv.Mat();
  const gradX = new cv.Mat();
  const gradY = new cv.Mat();
  const absGradX = new cv.Mat();
  const absGradY = new cv.Mat();
  try {
    cv.cvtColor(dst, gray, cv.COLOR_RGBA2GRAY);
    cv.Sobel(gray, gradX, cv.CV_16S, 1, 0);
    cv.Sobel(gray, gradY, cv.CV_16S, 0, 1);
    cv.convertScaleAbs(gradX, absGradX);
    cv.convertScaleAbs(gradY, absGradY);
    cv.addWeighted(absGradX, 0.5, absGradY, 0.5, 0, dst);
    cv.cvtColor(dst, dst, cv.COLOR_GRAY2RGBA);
  } finally {
    gray.delete();
    gradX.delete();
    gradY.delete();
    absGradX.delete();
    absGradY.delete();
  }
};

const applyAdaptiveThreshold = (cv: CV, filters: FilterState, dst: Mat) => {
  if (!filters.adaptiveThreshold) return;

  const gray = new cv.Mat();
  const thresh = new cv.Mat();
  try {
    cv.cvtColor(dst, gray, cv.COLOR_RGBA2GRAY);
    cv.adaptiveThreshold(gray, thresh, 255, cv.ADAPTIVE_THRESH_GAUSSIAN_C, cv.THRESH_BINARY, 11, 2);
    cv.cvtColor(thresh, dst, cv.COLOR_GRAY2RGBA);
  } finally {
    gray.delete();
    thresh.delete();
  }
};

const applyOpening = (cv: CV, filters: FilterState, dst: Mat) => {
  if (!filters.opening) {
    return;
  }

  const gray = new cv.Mat();
  const opened = new cv.Mat();
  const kernel = cv.Mat.ones(3, 3, cv.CV_8U);
  try {
    cv.cvtColor(dst, gray, cv.COLOR_RGBA2GRAY);
    cv.morphologyEx(gray, opened, cv.MORPH_OPEN, kernel);
    cv.cvtColor(opened, dst, cv.COLOR_GRAY2RGBA);
  } finally {
    gray.delete();
    opened.delete();
    kernel.delete();
  }
};

const applyClosing = (cv: CV, filters: FilterState, dst: Mat) => {
  if (!filters.closing) {
    return;
  }

  const gray = new cv.Mat();
  const closed = new cv.Mat();
  const kernel = cv.Mat.ones(3, 3, cv.CV_8U);
  try {
    cv.cvtColor(dst, gray, cv.COLOR_RGBA2GRAY);
    cv.morphologyEx(gray, closed, cv.MORPH_CLOSE, kernel);
    cv.cvtColor(closed, dst, cv.COLOR_GRAY2RGBA);
  } finally {
    gray.delete();
    closed.delete();
    kernel.delete();
  }
};

const applySharpen = (cv: CV, filters: FilterState, dst: Mat) => {
  if (filters.sharpen <= 0) return;

  const kernel = cv.matFromArray(3, 3, cv.CV_32F, [0, -1, 0, -1, 5 + filters.sharpen, -1, 0, -1, 0]);

  try {
    cv.filter2D(dst, dst, cv.CV_8U, kernel);
  } finally {
    kernel.delete();
  }
};

const applyFlip = (cv: CV, transforms: TransformState, dst: Mat) => {
  if (!transforms.flipH && !transforms.flipV) return;

  let flipCode = 1;
  if (transforms.flipH && transforms.flipV) flipCode = -1;
  else if (transforms.flipV) flipCode = 0;

  cv.flip(dst, dst, flipCode);
};

const applyRotationAndScale = (cv: CV, transforms: TransformState, dst: Mat) => {
  if (transforms.rotation === 0 && transforms.scale === 1) return;

  const center = new cv.Point(dst.cols / 2, dst.rows / 2);
  const rotMat = cv.getRotationMatrix2D(center, transforms.rotation, transforms.scale);
  try {
    cv.warpAffine(dst, dst, rotMat, new cv.Size(dst.cols, dst.rows));
  } finally {
    rotMat.delete();
  }
};

const runProcessingPipeline = ({ cv, filters, transforms, img, canvas, setProcessing }: ProcessingParams) => {
  setProcessing(true);

  let src: Mat | null = null;
  let dst: Mat | null = null;

  try {
    drawImageElementToCanvas(canvas, img);
    const ctx = get2DContextOrThrow(canvas, { willReadFrequently: true });

    const imageData = ctx.getImageData(0, 0, canvas.width, canvas.height);
    src = cv.matFromImageData(imageData);
    dst = src.clone();

    applyGrayscale(cv, filters, dst);
    applyDenoise(cv, filters, dst);
    applyBlur(cv, filters, dst);
    applyBrightnessContrast(filters, dst);
    applySaturationAndHue(cv, filters, dst);
    applyThreshold(cv, filters, dst);
    applyEdgeDetection(cv, filters, dst);
    applySobelEdgeDetection(cv, filters, dst);
    applyAdaptiveThreshold(cv, filters, dst);
    applyOpening(cv, filters, dst);
    applyClosing(cv, filters, dst);
    applySharpen(cv, filters, dst);
    applyFlip(cv, transforms, dst);
    applyRotationAndScale(cv, transforms, dst);

    cv.imshow(canvas, dst);
  } catch (err) {
    console.error("Processing error:", err);
  } finally {
    if (src && !src.isDeleted()) src.delete();
    if (dst && !dst.isDeleted()) dst.delete();
    setProcessing(false);
  }
};

export const useProcessingPipeline = ({
  cv,
  filters,
  transforms,
  originalImageRef,
  canvasRef,
  comparisonCanvasRef,
  showComparison,
  setProcessing,
  skipNextProcessingRef
}: Args) => {
  useEffect(() => {
    if (skipNextProcessingRef?.current) {
      // eslint-disable-next-line react-compiler/react-compiler
      skipNextProcessingRef.current = false;
      return;
    }

    const img = originalImageRef.current;
    const baseCanvas = canvasRef.current;

    if (img && baseCanvas) {
      runProcessingPipeline({
        cv,
        filters,
        transforms,
        img,
        canvas: baseCanvas,
        setProcessing
      });
    }

    if (showComparison && comparisonCanvasRef.current && originalImageRef.current) {
      drawImageElementToCanvas(comparisonCanvasRef.current, originalImageRef.current);
    }
  }, [
    cv,
    filters,
    transforms,
    originalImageRef,
    canvasRef,
    comparisonCanvasRef,
    showComparison,
    setProcessing,
    skipNextProcessingRef
  ]);
};
