"use client";

import type { MouseEvent, RefObject } from "react";

import type { DrawingTool } from "../types";

export type Point = { x: number; y: number };

export type CropRect = {
  startX: number;
  startY: number;
  endX: number;
  endY: number;
};

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

export const getCanvasCoordinates = (e: MouseEvent<HTMLCanvasElement>, canvas: HTMLCanvasElement): Point => {
  const rect = canvas.getBoundingClientRect();
  const x = (e.clientX - rect.left) * (canvas.width / rect.width);
  const y = (e.clientY - rect.top) * (canvas.height / rect.height);
  return { x, y };
};

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

export const isShapeTool = (tool: DrawingTool) => tool === "rectangle" || tool === "circle" || tool === "line";

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

export const normalizeCropRect = (crop: CropRect) => {
  const x = Math.min(crop.startX, crop.endX);
  const y = Math.min(crop.startY, crop.endY);
  const width = Math.abs(crop.endX - crop.startX);
  const height = Math.abs(crop.endY - crop.startY);
  return { x, y, width, height };
};

export const beginStroke = (ctx: CanvasRenderingContext2D, point: Point, color: string, brushSize: number) => {
  ctx.strokeStyle = color;
  ctx.fillStyle = color;
  ctx.lineWidth = brushSize;
  ctx.lineCap = "round";
  ctx.globalCompositeOperation = "source-over";
  ctx.beginPath();
  ctx.moveTo(point.x, point.y);
};

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

export const getCanvasDataUrl = (canvasRef: RefObject<HTMLCanvasElement | null>, type = "image/png"): string => {
  const canvas = canvasRef.current;
  if (!canvas) return "";
  return canvas.toDataURL(type);
};

export const drawImageElementToCanvas = (canvas: HTMLCanvasElement, img: HTMLImageElement) => {
  canvas.width = img.width;
  canvas.height = img.height;

  const ctx = canvas.getContext("2d");
  if (!ctx) return;

  ctx.clearRect(0, 0, canvas.width, canvas.height);
  ctx.drawImage(img, 0, 0);
};

export const drawDataUrlToCanvas = (canvas: HTMLCanvasElement, dataUrl: string) => {
  if (!dataUrl) return;

  const img = new Image();
  img.onload = () => {
    drawImageElementToCanvas(canvas, img);
  };
  img.src = dataUrl;
};

export const drawCanvasToCanvas = (target: HTMLCanvasElement, source: HTMLCanvasElement) => {
  target.width = source.width;
  target.height = source.height;

  const ctx = target.getContext("2d");
  if (!ctx) return;

  ctx.clearRect(0, 0, target.width, target.height);
  ctx.drawImage(source, 0, 0);
};
