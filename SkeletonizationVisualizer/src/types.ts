/**
 * @file types.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Declares shared benchmark visualization data types.
 * @description Defines metrics, image containers, modal state, and theme primitives used across the UI.
 * @version 1.0
 * @date 2026-03-16
 */

/**
 * @brief Represents execution and image-level benchmark measurements.
 */
export type BenchmarkMetrics = {
  // Google Benchmark specific
  iterations?: number;
  realTime?: number; // nanoseconds
  cpuTime?: number; // nanoseconds
  timeUnit?: string; // "ns", "us", "ms", "s"
  bytesPerSecond?: number;
  itemsPerSecond?: number;

  // Custom metrics
  executionTimeMs?: number;
  memoryUsageMB?: number;
  pixelCount?: number;
  nonZeroPixels?: number;
  compressionRatio?: number;
};

/**
 * @brief Represents one renderable image with optional benchmark metadata.
 */
export type ImageData = {
  id: string;
  label: string;
  data: string; // Base64 encoded image data
  width: number;
  height: number;
  metrics?: BenchmarkMetrics;
};

/**
 * @brief Describes aggregate statistics for one algorithm entry.
 */
export type AlgorithmStats = {
  description?: string;
  avgTime?: number;
  minTime?: number;
  maxTime?: number;
  totalIterations?: number;
  avgIterations?: number;
  stdDev?: number;
  throughput?: number; // images/second
  complexity?: string; // "O(n)", "O(n²)", etc.
};

/**
 * @brief Groups original and processed images for a single benchmark case.
 */
export type ImageContainer = {
  name: string;
  images: ImageData[];
  algorithmInfo?: AlgorithmStats;
};

/**
 * @brief Defines the root benchmark payload consumed by the visualizer.
 */
export type BenchmarkData = {
  timestamp: string;
  containers: ImageContainer[];
};

/**
 * @brief Stores zoom and pan transform state for modal image viewing.
 */
export type ZoomState = {
  scale: number;
  translateX: number;
  translateY: number;
};

/**
 * @brief Enumerates available visual comparison modes.
 */
export type ComparisonMode = "side-by-side" | "slider" | "overlay";

/**
 * @brief Enumerates supported visual themes.
 */
export type Theme = "light" | "dark";
