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

export type ImageData = {
  id: string;
  label: string;
  data: string; // Base64 encoded image data
  width: number;
  height: number;
  metrics?: BenchmarkMetrics;
};

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

export type ImageContainer = {
  name: string;
  images: ImageData[];
  algorithmInfo?: AlgorithmStats;
};

export type BenchmarkData = {
  timestamp: string;
  containers: ImageContainer[];
};

export type ZoomState = {
  scale: number;
  translateX: number;
  translateY: number;
};

export type ViewMode = "gallery" | "comparison" | "slideshow";
export type ComparisonMode = "side-by-side" | "slider" | "overlay";
export type Theme = "light" | "dark";
