/**
 * @file use-drawing.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Manages interactive drawing and crop behavior on preprocessing canvas.
 * @description Handles pointer events for freehand tools, shapes, flood fill, and crop operations while committing changes to history.
 */

"use client";

import { type CV, type Point } from "mirada/dist/src/types/opencv";
import { type MouseEvent, type RefObject, useRef, useState } from "react";

import type { DrawingTool } from "../types";
import {
  beginStroke,
  continueStroke,
  type CropRect,
  drawCropOverlay,
  drawShape,
  ensureTempCanvasSnapshot,
  floodFillCanvas,
  getCanvasCoordinates,
  isShapeTool,
  normalizeCropRect
} from "../utils/canvas";

type UseDrawingArgs = {
  canvasRef: RefObject<HTMLCanvasElement>;
  cv: CV;
  activeTool: DrawingTool;
  drawColor: string;
  brushSize: number;
  commitAsProcessingBase: () => void;
  addToHistory: (description: string) => void;
  onFinishCrop: () => void;
};

type CropState = CropRect & { active: boolean };

const initialCropState: CropState = {
  active: false,
  startX: 0,
  startY: 0,
  endX: 0,
  endY: 0
};

/**
 * @brief Provides canvas mouse handlers for drawing workflows.
 * @description Orchestrates drawing tool logic, temporary snapshots, crop execution, and post-operation history updates.
 * @param canvasRef Reference to the active output canvas.
 * @param cv OpenCV runtime instance used for crop operations.
 * @param activeTool Selected drawing tool mode.
 * @param drawColor Current drawing color value.
 * @param brushSize Current brush size in pixels.
 * @param commitAsProcessingBase Callback to sync edited output into processing base canvas.
 * @param addToHistory Callback to append user action history.
 * @param onFinishCrop Callback invoked when crop flow ends.
 * @returns Mouse event handlers for canvas drawing interactions.
 */
/** @brief Provides mouse handlers for preprocessing drawing tools. */
export const useDrawing = ({
  canvasRef,
  cv,
  activeTool,
  drawColor,
  brushSize,
  commitAsProcessingBase,
  addToHistory,
  onFinishCrop
}: UseDrawingArgs) => {
  const [isDrawing, setIsDrawing] = useState(false);
  const [shapeStart, setShapeStart] = useState<Point | null>(null);
  const [cropState, setCropState] = useState<CropState>(initialCropState);

  const tempCanvasRef = useRef<HTMLCanvasElement | null>(null);

  const resetCropState = () => setCropState(initialCropState);

  const applyCrop = () => {
    const canvas = canvasRef.current;
    const tempCanvas = tempCanvasRef.current;
    if (!canvas || !tempCanvas) return;

    const { x, y, width, height } = normalizeCropRect(cropState);

    if (width < 10 || height < 10) {
      const ctx = canvas.getContext("2d");
      if (ctx) {
        ctx.drawImage(tempCanvas, 0, 0);
      }
      resetCropState();
      return;
    }

    try {
      const src = cv.imread(tempCanvas);
      const rect = new cv.Rect(Math.floor(x), Math.floor(y), Math.floor(width), Math.floor(height));
      const cropped = src.roi(rect);
      cv.imshow(canvas, cropped);
      commitAsProcessingBase();
      src.delete();
      cropped.delete();
      addToHistory("Cropped image");
    } catch (err) {
      console.error("Crop error:", err);
    }

    resetCropState();
    onFinishCrop();
  };

  const handleCanvasMouseDown = (e: MouseEvent<HTMLCanvasElement>) => {
    const canvas = canvasRef.current;
    if (!canvas || activeTool === "none") return;

    const point = getCanvasCoordinates(e, canvas);

    if (activeTool === "fill") {
      const filled = floodFillCanvas(canvas, point.x, point.y, drawColor);
      if (filled) {
        commitAsProcessingBase();
        addToHistory("Fill applied");
      }
      return;
    }

    if (activeTool === "crop") {
      setCropState({
        active: true,
        startX: point.x,
        startY: point.y,
        endX: point.x,
        endY: point.y
      });
      tempCanvasRef.current = ensureTempCanvasSnapshot(tempCanvasRef.current, canvas);
      return;
    }

    if (isShapeTool(activeTool)) {
      setIsDrawing(true);
      setShapeStart(point);
      tempCanvasRef.current = ensureTempCanvasSnapshot(tempCanvasRef.current, canvas);
      return;
    }

    const ctx = canvas.getContext("2d");
    if (!ctx) return;

    setIsDrawing(true);
    beginStroke(ctx, point, drawColor, brushSize);
  };

  const handleCanvasMouseMove = (e: MouseEvent<HTMLCanvasElement>) => {
    const canvas = canvasRef.current;
    if (!canvas) return;

    const point = getCanvasCoordinates(e, canvas);

    if (activeTool === "crop" && cropState.active) {
      const nextCrop: CropRect = {
        startX: cropState.startX,
        startY: cropState.startY,
        endX: point.x,
        endY: point.y
      };
      setCropState((prev) => ({
        ...prev,
        endX: point.x,
        endY: point.y
      }));

      const ctx = canvas.getContext("2d");
      if (ctx && tempCanvasRef.current) {
        ctx.drawImage(tempCanvasRef.current, 0, 0);
        drawCropOverlay(ctx, canvas, nextCrop);
      }
      return;
    }

    if (isDrawing && isShapeTool(activeTool) && shapeStart && tempCanvasRef.current) {
      const ctx = canvas.getContext("2d");
      if (!ctx) return;

      ctx.drawImage(tempCanvasRef.current, 0, 0);
      ctx.strokeStyle = drawColor;
      ctx.fillStyle = drawColor;
      ctx.lineWidth = brushSize;

      drawShape(ctx, activeTool, shapeStart, point);
      return;
    }

    if (isDrawing && (activeTool === "pencil" || activeTool === "eraser")) {
      const ctx = canvas.getContext("2d");
      if (!ctx) return;

      continueStroke(ctx, activeTool, point, brushSize, drawColor);
    }
  };

  const handleCanvasMouseUp = (e: MouseEvent<HTMLCanvasElement>) => {
    const canvas = canvasRef.current;
    if (!canvas) return;

    if (activeTool === "crop" && cropState.active) {
      applyCrop();
      setIsDrawing(false);
      return;
    }

    const point = getCanvasCoordinates(e, canvas);

    if (isDrawing && isShapeTool(activeTool) && shapeStart && tempCanvasRef.current) {
      const ctx = canvas.getContext("2d");
      if (ctx) {
        ctx.drawImage(tempCanvasRef.current, 0, 0);
        ctx.strokeStyle = drawColor;
        ctx.fillStyle = drawColor;
        ctx.lineWidth = brushSize;

        drawShape(ctx, activeTool, shapeStart, point);
        commitAsProcessingBase();
        addToHistory(`Drew ${activeTool}`);
      }
      setShapeStart(null);
      setIsDrawing(false);
      return;
    }

    if (isDrawing && (activeTool === "pencil" || activeTool === "eraser")) {
      commitAsProcessingBase();
      addToHistory(activeTool === "pencil" ? "Drew with pencil" : "Erased");
    }

    setIsDrawing(false);
  };

  const handleCanvasMouseLeave = (e: MouseEvent<HTMLCanvasElement>) => {
    if (isDrawing) {
      handleCanvasMouseUp(e);
    }
  };

  return {
    onMouseDown: handleCanvasMouseDown,
    onMouseMove: handleCanvasMouseMove,
    onMouseUp: handleCanvasMouseUp,
    onMouseLeave: handleCanvasMouseLeave
  };
};
