/**
 * @file index.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Re-exports statistics utility APIs.
 * @description Exposes calculation function and related statistic types through a single entry point.
 * @version 1.0
 * @date 2026-03-16
 */

export { calculateBenchmarkStats, type AlgorithmAvgTime, type BenchmarkStats } from "./calculate-stats";
