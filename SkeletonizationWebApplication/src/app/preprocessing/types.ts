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

export type TransformState = {
  rotation: number;
  flipH: boolean;
  flipV: boolean;
  scale: number;
};

export type DrawingTool = "none" | "pencil" | "eraser" | "rectangle" | "circle" | "line" | "text" | "crop" | "fill";

export type CropState = {
  active: boolean;
  startX: number;
  startY: number;
  endX: number;
  endY: number;
};

export type HistoryEntry = {
  filters: FilterState;
  transforms: TransformState;
  canvasDataUrl: string;
  timestamp: number;
  description: string;
};

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

export const initialTransforms: TransformState = {
  rotation: 0,
  flipH: false,
  flipV: false,
  scale: 1
};
