/**
 * @file types.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Defines shared preprocessing state types and defaults.
 * @description Declares filter, transform, drawing, crop, and history models used across preprocessing hooks and components.
 */

/**
 * @brief Represents adjustable image filter settings.
 * @description Stores all filter flags and parameter values used by the preprocessing pipeline.
 */
export type FilterState = {
  blur: number;
  sharpen: number;
  brightness: number;
  contrast: number;
  thresholding: boolean;
  threshold: number;
  edgeDetection: boolean;
  sobelEdgeDetection: boolean;
  adaptiveThreshold: boolean;
  opening: boolean;
  closing: boolean;
  grayscale: boolean;
  denoise: number;
  saturation: number;
  hue: number;
};

/**
 * @brief Represents geometric transform settings.
 * @description Tracks rotation, flipping, and scaling values applied to preview output.
 */
export type TransformState = {
  rotation: number;
  flipH: boolean;
  flipV: boolean;
  scale: number;
};

/**
 * @brief Enumerates available drawing tools.
 * @description Identifies the currently selected annotation mode for canvas editing.
 */
export type DrawingTool = "none" | "pencil" | "eraser" | "rectangle" | "circle" | "line" | "text" | "crop" | "fill";

/**
 * @brief Stores active crop interaction bounds.
 * @description Maintains the start and end coordinates for a crop selection gesture.
 */
export type CropState = {
  active: boolean;
  startX: number;
  startY: number;
  endX: number;
  endY: number;
};

/**
 * @brief Captures a preprocessing history snapshot.
 * @description Persists state and canvas data needed for undo/redo operations.
 */
export type HistoryEntry = {
  filters: FilterState;
  transforms: TransformState;
  canvasDataUrl: string;
  baseCanvasDataUrl: string;
  timestamp: number;
  description: string;
};

/**
 * @brief Provides default filter values for workspace initialization.
 * @description Defines neutral settings that preserve original image appearance before edits.
 */
export const initialFilters: FilterState = {
  blur: 0,
  sharpen: 0,
  brightness: 0,
  contrast: 1,
  thresholding: false,
  threshold: 0,
  edgeDetection: false,
  sobelEdgeDetection: false,
  adaptiveThreshold: false,
  opening: false,
  closing: false,
  grayscale: false,
  denoise: 0,
  saturation: 1,
  hue: 0
};

/**
 * @brief Provides default transform values for workspace initialization.
 * @description Initializes transforms to identity so no geometric changes are applied initially.
 */
export const initialTransforms: TransformState = {
  rotation: 0,
  flipH: false,
  flipV: false,
  scale: 1
};
