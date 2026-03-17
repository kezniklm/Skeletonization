/**
 * @file calculate-stats.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Implements benchmark statistics aggregation logic.
 * @description Computes aggregate timings, iteration totals, and per-algorithm ordering from benchmark containers.
 * @version 1.0
 * @date 2026-03-16
 */

import type { BenchmarkData } from "../../../types";

/**
 * @brief Represents aggregated timing information for one algorithm.
 */
export type AlgorithmAvgTime = {
  name: string;
  avgTime: number;
  minTime: number;
  maxTime: number;
  runs: number;
};

/**
 * @brief Represents computed benchmark dashboard metrics.
 */
export type BenchmarkStats = {
  totalTestImages: number;
  totalAlgorithms: number;
  avgTime: number;
  minTime: number;
  maxTime: number;
  totalIterations: number;
  algorithmAvgTimes: AlgorithmAvgTime[];
  fastest: AlgorithmAvgTime | undefined;
  slowest: AlgorithmAvgTime | undefined;
};

/**
 * @brief Calculates aggregate benchmark statistics from raw containers.
 * @param data Benchmark data source.
 * @returns Computed statistics object for dashboard rendering.
 */
export const calculateBenchmarkStats = (data: BenchmarkData): BenchmarkStats => {
  const algorithmTimes = new Map<string, number[]>();
  const algorithmIterations = new Map<string, number>();

  for (const container of data.containers) {
    for (const img of container.images) {
      if (!img.metrics || !img.label || img.label === "Original") {
        continue;
      }

      const algorithmName = img.label;
      const execTime = img.metrics.executionTimeMs ?? (img.metrics.cpuTime ? img.metrics.cpuTime / 1000000 : 0);

      if (execTime > 0) {
        if (!algorithmTimes.has(algorithmName)) {
          algorithmTimes.set(algorithmName, []);
          algorithmIterations.set(algorithmName, 0);
        }
        algorithmTimes.get(algorithmName)!.push(execTime);
        algorithmIterations.set(algorithmName, algorithmIterations.get(algorithmName)! + (img.metrics.iterations ?? 0));
      }
    }
  }

  const totalTestImages = data.containers.length;
  const totalAlgorithms = algorithmTimes.size;

  const allTimes = Array.from(algorithmTimes.values()).flat();
  const avgTime = allTimes.length > 0 ? allTimes.reduce((a, b) => a + b, 0) / allTimes.length : 0;
  const minTime = allTimes.length > 0 ? Math.min(...allTimes) : 0;
  const maxTime = allTimes.length > 0 ? Math.max(...allTimes) : 0;

  const totalIterations = Array.from(algorithmIterations.values()).reduce((sum, i) => sum + i, 0);

  const algorithmAvgTimes = Array.from(algorithmTimes.entries())
    .map(([name, times]) => ({
      name,
      avgTime: times.reduce((a, b) => a + b, 0) / times.length,
      minTime: Math.min(...times),
      maxTime: Math.max(...times),
      runs: times.length
    }))
    .sort((a, b) => a.avgTime - b.avgTime);

  const fastest = algorithmAvgTimes[0];
  const slowest = algorithmAvgTimes[algorithmAvgTimes.length - 1];

  return {
    totalTestImages,
    totalAlgorithms,
    avgTime,
    minTime,
    maxTime,
    totalIterations,
    algorithmAvgTimes,
    fastest,
    slowest
  };
};
