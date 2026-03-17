/**
 * @file useBenchmarkData.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Implements benchmark data loading hook.
 * @description Fetches benchmark JSON payload and refreshes it on a fixed interval.
 * @version 1.0
 * @date 2026-03-16
 */

import { useEffect, useState } from "react";

import { APP_CONFIG } from "../../../constants";
import type { BenchmarkData } from "../../../types";

/**
 * @brief Loads and periodically refreshes benchmark dataset.
 * @returns Data, loading flag, and error state.
 * @example
 * const { data, loading, error } = useBenchmarkData();
 */
export const useBenchmarkData = () => {
  const [data, setData] = useState<BenchmarkData | null>(null);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    const loadData = async () => {
      try {
        const response = await fetch("benchmark_data.json");
        if (!response.ok) {
          throw new Error("Failed to load benchmark data");
        }
        const jsonData: BenchmarkData = await response.json();
        setData(jsonData);
        setLoading(false);
      } catch (err) {
        console.error("Error loading data:", err);
        setError("Failed to load benchmark data. Make sure the data file exists.");
        setLoading(false);
      }
    };

    loadData();
    const interval = setInterval(loadData, APP_CONFIG.DATA_REFRESH_INTERVAL);
    return () => clearInterval(interval);
  }, []);

  return { data, loading, error };
};
