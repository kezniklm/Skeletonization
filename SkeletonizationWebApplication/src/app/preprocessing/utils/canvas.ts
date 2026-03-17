/**
 * @file canvas.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Provides shared canvas helpers for preprocessing workflows.
 * @description Implements coordinate conversion, drawing primitives, flood fill, serialization, and canvas-to-canvas rendering utilities.
 */

"use client";

import type { MouseEvent, RefObject } from "react";

import type { DrawingTool } from "../types";

/**
 * @brief Represents a 2D point in canvas coordinates.
 */
export type Point = { x: number; y: number };

/**
 * @brief Represents a crop rectangle using start and end coordinates.
 */
export type CropRect = {
  startX: number;
  startY: number;
  endX: number;
  endY: number;
};

/**
 * @brief Resolves a canvas element from a ref or throws an error.
 * @description Ensures downstream canvas operations always receive a non-null canvas element.
 * @param canvasRef React ref for a canvas element.
 * @param message Error message used when canvas is unavailable.
 * @returns The resolved canvas element.
 */
export const getCanvasOrThrow = (
  canvasRef: RefObject<HTMLCanvasElement>,
  message = "Canvas element is not available."
): HTMLCanvasElement => {
  const canvas = canvasRef.current;
  if (!canvas) {
    throw new Error(message);
  }
  return canvas;
};

/**
 * @brief Resolves a 2D context from a canvas or throws an error.
 * @description Acquires the rendering context with optional settings required by preprocessing operations.
 * @param canvas Canvas element to query.
 * @param options Optional context creation settings.
 * @param message Error message used when context cannot be created.
 * @returns A 2D canvas rendering context.
 */
export const get2DContextOrThrow = (
  canvas: HTMLCanvasElement,
  options: CanvasRenderingContext2DSettings = { willReadFrequently: true },
  message = "Unable to get 2D rendering context."
): CanvasRenderingContext2D => {
  const ctx = canvas.getContext("2d", options);
  if (!ctx) {
    throw new Error(message);
  }
  return ctx;
};

/**
 * @brief Converts pointer event coordinates into canvas pixel coordinates.
 * @description Maps client-space mouse coordinates to canvas-space values accounting for element scaling.
 * @param e Mouse event emitted by the canvas.
 * @param canvas Target canvas element.
 * @returns Canvas-space point coordinates.
 */
export const getCanvasCoordinates = (e: MouseEvent<HTMLCanvasElement>, canvas: HTMLCanvasElement): Point => {
  const rect = canvas.getBoundingClientRect();
  const x = (e.clientX - rect.left) * (canvas.width / rect.width);
  const y = (e.clientY - rect.top) * (canvas.height / rect.height);
  return { x, y };
};

/**
 * @brief Creates or refreshes a temporary canvas snapshot.
 * @description Copies source canvas pixels into a reusable offscreen canvas used for previewing transient drawing operations.
 * @param tempCanvas Existing temp canvas instance, if any.
 * @param sourceCanvas Canvas to snapshot.
 * @returns The snapshot canvas or `null` when context creation fails.
 */
export const ensureTempCanvasSnapshot = (
  tempCanvas: HTMLCanvasElement | null,
  sourceCanvas: HTMLCanvasElement
): HTMLCanvasElement | null => {
  tempCanvas ??= document.createElement("canvas");

  tempCanvas.width = sourceCanvas.width;
  tempCanvas.height = sourceCanvas.height;

  const tempCtx = tempCanvas.getContext("2d");
  if (!tempCtx) return null;

  tempCtx.clearRect(0, 0, tempCanvas.width, tempCanvas.height);
  tempCtx.drawImage(sourceCanvas, 0, 0);

  return tempCanvas;
};

/**
 * @brief Checks whether a drawing tool is shape-based.
 * @description Identifies tools rendered from start/end points rather than continuous strokes.
 * @param tool Drawing tool value.
 * @returns `true` for rectangle, circle, and line tools.
 */
export const isShapeTool = (tool: DrawingTool) => tool === "rectangle" || tool === "circle" || tool === "line";

/**
 * @brief Draws a shape between start and end points.
 * @description Renders rectangle, circle, or line previews on the provided context.
 * @param ctx Target 2D rendering context.
 * @param tool Active drawing tool.
 * @param start Starting point.
 * @param end Current or ending point.
 * @returns No return value.
 */
export const drawShape = (ctx: CanvasRenderingContext2D, tool: DrawingTool, start: Point, end: Point) => {
  if (!isShapeTool(tool)) return;

  if (tool === "rectangle") {
    const width = end.x - start.x;
    const height = end.y - start.y;
    ctx.strokeRect(start.x, start.y, width, height);
    return;
  }

  if (tool === "circle") {
    const radius = Math.sqrt(Math.pow(end.x - start.x, 2) + Math.pow(end.y - start.y, 2));
    ctx.beginPath();
    ctx.arc(start.x, start.y, radius, 0, 2 * Math.PI);
    ctx.stroke();
    return;
  }

  if (tool === "line") {
    ctx.beginPath();
    ctx.moveTo(start.x, start.y);
    ctx.lineTo(end.x, end.y);
    ctx.stroke();
  }
};

/**
 * @brief Draws a crop overlay mask and selection frame.
 * @description Shades non-selected regions and outlines the normalized crop rectangle.
 * @param ctx Target 2D rendering context.
 * @param canvas Canvas element receiving overlay rendering.
 * @param crop Raw crop bounds.
 * @returns No return value.
 */
export const drawCropOverlay = (ctx: CanvasRenderingContext2D, canvas: HTMLCanvasElement, crop: CropRect) => {
  const { x, y, width, height } = normalizeCropRect(crop);

  ctx.fillStyle = "rgba(0, 0, 0, 0.5)";
  ctx.fillRect(0, 0, canvas.width, y);
  ctx.fillRect(0, y, x, height);
  ctx.fillRect(x + width, y, canvas.width - x - width, height);
  ctx.fillRect(0, y + height, canvas.width, canvas.height - y - height);

  ctx.strokeStyle = "#00ff00";
  ctx.lineWidth = 2;
  ctx.strokeRect(x, y, width, height);
};

/**
 * @brief Normalizes crop coordinates into top-left origin and positive size.
 * @description Converts drag bounds into canonical rectangle values.
 * @param crop Raw crop rectangle.
 * @returns Normalized crop rectangle with `x`, `y`, `width`, and `height`.
 */
export const normalizeCropRect = (crop: CropRect) => {
  const x = Math.min(crop.startX, crop.endX);
  const y = Math.min(crop.startY, crop.endY);
  const width = Math.abs(crop.endX - crop.startX);
  const height = Math.abs(crop.endY - crop.startY);
  return { x, y, width, height };
};

/**
 * @brief Initializes drawing stroke settings and starting point.
 * @description Configures stroke styles and begins a new path for freehand drawing.
 * @param ctx Target 2D rendering context.
 * @param point Initial stroke point.
 * @param color Stroke color.
 * @param brushSize Brush width in pixels.
 * @returns No return value.
 */
export const beginStroke = (ctx: CanvasRenderingContext2D, point: Point, color: string, brushSize: number) => {
  ctx.strokeStyle = color;
  ctx.fillStyle = color;
  ctx.lineWidth = brushSize;
  ctx.lineCap = "round";
  ctx.globalCompositeOperation = "source-over";
  ctx.beginPath();
  ctx.moveTo(point.x, point.y);
};

/**
 * @brief Continues a freehand stroke for pencil or eraser tools.
 * @description Updates compositing and stroke properties before drawing to the next point.
 * @param ctx Target 2D rendering context.
 * @param tool Active drawing tool.
 * @param point Next stroke point.
 * @param brushSize Brush width in pixels.
 * @param color Stroke color.
 * @returns No return value.
 */
/** @brief Continues a freehand stroke on the canvas. */
export const continueStroke = (
  ctx: CanvasRenderingContext2D,
  tool: DrawingTool,
  point: Point,
  brushSize: number,
  color: string
) => {
  ctx.globalCompositeOperation = "source-over";
  if (tool === "eraser") {
    ctx.strokeStyle = color;
    ctx.lineWidth = brushSize * 2;
  }
  ctx.lineTo(point.x, point.y);
  ctx.stroke();
};

/**
 * @brief Performs flood fill on a canvas pixel region.
 * @description Fills connected pixels near the start point using color matching with tolerance.
 * @param canvas Canvas element to mutate.
 * @param startX Fill start X coordinate.
 * @param startY Fill start Y coordinate.
 * @param fillColor Target fill color.
 * @param tolerance Color tolerance threshold.
 * @returns `true` when fill modifies the canvas; otherwise `false`.
 */
/** @brief Flood-fills connected canvas pixels. */
export const floodFillCanvas = (
  canvas: HTMLCanvasElement,
  startX: number,
  startY: number,
  fillColor: string,
  tolerance = 32
): boolean => {
  const ctx = canvas.getContext("2d", { willReadFrequently: true });
  if (!ctx) return false;

  const imageData = ctx.getImageData(0, 0, canvas.width, canvas.height);
  const data = imageData.data;
  const width = canvas.width;
  const height = canvas.height;

  const tempCanvas = document.createElement("canvas");
  const tempCtx = tempCanvas.getContext("2d");
  if (!tempCtx) return false;
  tempCtx.fillStyle = fillColor;
  tempCtx.fillRect(0, 0, 1, 1);
  const fillData = tempCtx.getImageData(0, 0, 1, 1).data;
  const [fillR, fillG, fillB, fillA] = fillData;

  const sx = Math.floor(startX);
  const sy = Math.floor(startY);
  const startIdx = (sy * width + sx) * 4;

  const targetR = data[startIdx];
  const targetG = data[startIdx + 1];
  const targetB = data[startIdx + 2];
  const targetA = data[startIdx + 3];

  const isSameAsFillColor = targetR === fillR && targetG === fillG && targetB === fillB && targetA === fillA;

  if (isSameAsFillColor) {
    return false;
  }

  const matchesTarget = (idx: number) =>
    Math.abs(data[idx] - targetR) <= tolerance &&
    Math.abs(data[idx + 1] - targetG) <= tolerance &&
    Math.abs(data[idx + 2] - targetB) <= tolerance &&
    Math.abs(data[idx + 3] - targetA) <= tolerance;

  const stack: [number, number][] = [[sx, sy]];
  const visited = new Set<string>();

  while (stack.length > 0) {
    const [x, y] = stack.pop()!;
    const key = `${x},${y}`;
    if (x < 0 || x >= width || y < 0 || y >= height || visited.has(key)) continue;
    visited.add(key);

    const idx = (y * width + x) * 4;
    if (!matchesTarget(idx)) continue;

    data[idx] = fillR;
    data[idx + 1] = fillG;
    data[idx + 2] = fillB;
    data[idx + 3] = fillA;

    stack.push([x + 1, y], [x - 1, y], [x, y + 1], [x, y - 1]);
  }

  ctx.putImageData(imageData, 0, 0);
  return true;
};

/**
 * @brief Serializes a canvas ref to a data URL.
 * @description Returns empty string when canvas is unavailable.
 * @param canvasRef React ref containing an optional canvas.
 * @param type Output MIME type.
 * @returns Canvas data URL string.
 */
export const getCanvasDataUrl = (canvasRef: RefObject<HTMLCanvasElement | null>, type = "image/png"): string => {
  const canvas = canvasRef.current;
  if (!canvas) return "";
  return canvas.toDataURL(type);
};

/**
 * @brief Draws an image element into a canvas.
 * @description Resizes the canvas to image dimensions and paints the image content.
 * @param canvas Target canvas element.
 * @param img Source image element.
 * @returns No return value.
 */
export const drawImageElementToCanvas = (canvas: HTMLCanvasElement, img: HTMLImageElement) => {
  canvas.width = img.width;
  canvas.height = img.height;

  const ctx = canvas.getContext("2d");
  if (!ctx) return;

  ctx.clearRect(0, 0, canvas.width, canvas.height);
  ctx.drawImage(img, 0, 0);
};

/**
 * @brief Draws a data URL image into a canvas.
 * @description Loads the encoded image and forwards rendering to `drawImageElementToCanvas`.
 * @param canvas Target canvas element.
 * @param dataUrl Encoded image data URL.
 * @returns No return value.
 */
export const drawDataUrlToCanvas = (canvas: HTMLCanvasElement, dataUrl: string) => {
  if (!dataUrl) return;

  const img = new Image();
  img.onload = () => {
    drawImageElementToCanvas(canvas, img);
  };
  img.src = dataUrl;
};

/**
 * @brief Copies pixels from one canvas to another.
 * @description Resizes target canvas to source dimensions before drawing source content.
 * @param target Destination canvas.
 * @param source Source canvas.
 * @returns No return value.
 */
export const drawCanvasToCanvas = (target: HTMLCanvasElement, source: HTMLCanvasElement) => {
  target.width = source.width;
  target.height = source.height;

  const ctx = target.getContext("2d");
  if (!ctx) return;

  ctx.clearRect(0, 0, target.width, target.height);
  ctx.drawImage(source, 0, 0);
};
