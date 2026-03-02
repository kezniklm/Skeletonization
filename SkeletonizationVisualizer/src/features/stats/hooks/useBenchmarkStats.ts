import { useMemo } from "react";

import type { BenchmarkData } from "../../../types";
import { calculateBenchmarkStats, type BenchmarkStats } from "../utils";

export const useBenchmarkStats = (data: BenchmarkData): BenchmarkStats =>
  useMemo(() => calculateBenchmarkStats(data), [data]);
