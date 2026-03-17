/**
 * @file useBenchmarkStats.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Provides memoized benchmark statistics hook.
 * @description Computes and memoizes aggregated benchmark statistics for dashboard rendering.
 * @version 1.0
 * @date 2026-03-16
 */

import { useMemo } from "react";

import type { BenchmarkData } from "../../../types";
import { calculateBenchmarkStats, type BenchmarkStats } from "../utils";

/**
 * @brief Returns memoized benchmark statistics for input dataset.
 * @param data Benchmark data source.
 * @returns Aggregated benchmark statistics.
 */
export const useBenchmarkStats = (data: BenchmarkData): BenchmarkStats =>
  useMemo(() => calculateBenchmarkStats(data), [data]);
