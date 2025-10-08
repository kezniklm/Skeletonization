import React from "react";

import type { ImageContainer, BenchmarkData, ImageData } from "../types";
import { useTheme } from "../contexts/ThemeContext";

type StatsProps = {
  data: BenchmarkData;
};

export const StatsDashboard: React.FC<StatsProps> = ({ data }) => {
  const { theme, getThemeClasses } = useTheme();
  const themeClasses = getThemeClasses();

  const algorithmTimes = new Map<string, number[]>();
  const algorithmIterations = new Map<string, number>();

  data.containers.forEach((container) => {
    container.images.forEach((img) => {
      if (!img.metrics || !img.label || img.label === "Original") {
        return;
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
    });
  });

  // Calculate statistics
  const totalTestImages = data.containers.length; // Number of test images (Circle, Leaf A, etc.)
  const totalAlgorithms = algorithmTimes.size;

  const allTimes = Array.from(algorithmTimes.values()).flat();
  const avgTime = allTimes.length > 0 ? allTimes.reduce((a, b) => a + b, 0) / allTimes.length : 0;
  const minTime = allTimes.length > 0 ? Math.min(...allTimes) : 0;
  const maxTime = allTimes.length > 0 ? Math.max(...allTimes) : 0;

  const totalIterations = Array.from(algorithmIterations.values()).reduce((sum, i) => sum + i, 0);

  // Calculate average time per algorithm (averaged across all test images)
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

  return (
    <div className="mx-auto max-w-7xl px-4 py-6 sm:px-6 md:py-8 lg:px-8">
      <h2 className={`text-2xl font-bold md:text-3xl ${themeClasses.text} mb-6 flex items-center gap-3`}>
        📊 Benchmark Overview
      </h2>

      <div className="mb-8 grid grid-cols-1 gap-4 sm:grid-cols-2 md:gap-6 lg:grid-cols-4">
        <div className="transform rounded-xl bg-gradient-to-br from-purple-500 to-indigo-600 p-6 text-white shadow-lg transition-all hover:-translate-y-1.5 hover:shadow-xl">
          <div className="mb-3 text-4xl">🎯</div>
          <div>
            <div className="mb-2 text-3xl font-bold md:text-4xl">{totalAlgorithms}</div>
            <div className="text-sm opacity-90 md:text-base">Algorithms Tested</div>
          </div>
        </div>

        <div className="transform rounded-xl bg-gradient-to-br from-green-500 to-emerald-600 p-6 text-white shadow-lg transition-all hover:-translate-y-1.5 hover:shadow-xl">
          <div className="mb-3 text-4xl">🖼️</div>
          <div>
            <div className="mb-2 text-3xl font-bold md:text-4xl">{totalTestImages}</div>
            <div className="text-sm opacity-90 md:text-base">Test Images</div>
          </div>
        </div>

        <div className="transform rounded-xl bg-gradient-to-br from-blue-500 to-cyan-600 p-6 text-white shadow-lg transition-all hover:-translate-y-1.5 hover:shadow-xl">
          <div className="mb-3 text-4xl">⏱️</div>
          <div>
            <div className="mb-2 text-3xl font-bold md:text-4xl">{avgTime.toFixed(2)}ms</div>
            <div className="text-sm opacity-90 md:text-base">Average Time</div>
            <div className="mt-2 text-xs opacity-75">
              {minTime.toFixed(2)}ms - {maxTime.toFixed(2)}ms
            </div>
          </div>
        </div>

        <div className="transform rounded-xl bg-gradient-to-br from-orange-500 to-red-600 p-6 text-white shadow-lg transition-all hover:-translate-y-1.5 hover:shadow-xl">
          <div className="mb-3 text-4xl">🔄</div>
          <div>
            <div className="mb-2 text-3xl font-bold md:text-4xl">{totalIterations.toLocaleString()}</div>
            <div className="text-sm opacity-90 md:text-base">Total Iterations</div>
          </div>
        </div>
      </div>

      {/* Performance Comparison */}
      {fastest && slowest && (
        <div
          className={`${themeClasses.bgSecondary}/90 rounded-2xl border shadow-xl backdrop-blur-lg ${themeClasses.border}/50 mb-8 p-6`}
        >
          <h3 className={`text-xl font-bold md:text-2xl ${themeClasses.text} mb-6 flex items-center gap-2`}>
            ⚡ Performance Comparison
          </h3>
          <div className="grid grid-cols-1 gap-6 md:grid-cols-2">
            <div
              className={`bg-gradient-to-br ${theme === "dark" ? "border-green-700 from-green-900/20 to-emerald-900/20" : "border-green-500 from-green-50 to-emerald-50"} rounded-xl border-2 p-6 transition-all hover:-translate-y-1 hover:shadow-[0_8px_20px_rgba(16,185,129,0.25)]`}
            >
              <div className="mb-3 inline-block rounded-full bg-green-500 px-3 py-1 text-xs font-bold text-white">
                🏆 Fastest
              </div>
              <div className={`text-xl font-bold md:text-2xl ${themeClasses.text} mb-2`}>{fastest.name}</div>
              <div className="mb-4 text-2xl font-bold text-green-600 md:text-3xl dark:text-green-400">
                {fastest.avgTime.toFixed(2)}ms
              </div>
              <div className={`h-3 ${theme === "dark" ? "bg-gray-700" : "bg-gray-200"} overflow-hidden rounded-full`}>
                <div
                  className="h-full rounded-full bg-gradient-to-r from-green-500 to-emerald-500"
                  style={{ width: "100%" }}
                />
              </div>
            </div>

            <div
              className={`bg-gradient-to-br ${theme === "dark" ? "border-red-700 from-red-900/20 to-orange-900/20" : "border-red-500 from-red-50 to-orange-50"} rounded-xl border-2 p-6 transition-all hover:-translate-y-1 hover:shadow-[0_8px_20px_rgba(239,68,68,0.25)]`}
            >
              <div className="mb-3 inline-block rounded-full bg-red-500 px-3 py-1 text-xs font-bold text-white">
                🐌 Slowest
              </div>
              <div className={`text-xl font-bold md:text-2xl ${themeClasses.text} mb-2`}>{slowest.name}</div>
              <div
                className={`text-2xl font-bold md:text-3xl ${theme === "dark" ? "text-red-400" : "text-red-600"} mb-4`}
              >
                {slowest.avgTime.toFixed(2)}ms
              </div>
              <div
                className={`h-3 ${theme === "dark" ? "bg-gray-700" : "bg-gray-200"} mb-2 overflow-hidden rounded-full`}
              >
                <div
                  className="h-full rounded-full bg-gradient-to-r from-red-500 to-orange-500"
                  style={{ width: `${(fastest.avgTime / slowest.avgTime) * 100}%` }}
                />
              </div>
              <div className={`text-sm ${themeClasses.textMuted}`}>
                {((slowest.avgTime / fastest.avgTime - 1) * 100).toFixed(1)}% slower
              </div>
            </div>
          </div>
        </div>
      )}

      {/* Algorithm Rankings */}
      <div
        className={`${themeClasses.bgSecondary}/90 rounded-2xl border shadow-xl backdrop-blur-lg ${themeClasses.border}/50 p-6`}
      >
        <h3 className={`text-xl font-bold md:text-2xl ${themeClasses.text} mb-6 flex items-center gap-2`}>
          🏅 Algorithm Rankings
        </h3>
        <div className="space-y-4">
          {algorithmAvgTimes.map((algorithm, index) => (
            <div
              key={algorithm.name}
              className={`flex flex-col gap-3 p-4 md:flex-row md:items-center ${theme === "dark" ? "border-gray-700 bg-gradient-to-r from-[rgba(102,126,234,0.1)] to-[rgba(102,126,234,0.1)] hover:from-[rgba(102,126,234,0.15)] hover:to-[rgba(102,126,234,0.15)]" : "border-[rgba(102,126,234,0.15)] bg-gradient-to-r from-[rgba(102,126,234,0.03)] to-[rgba(139,155,255,0.02)] hover:from-[rgba(102,126,234,0.08)] hover:to-[rgba(139,155,255,0.06)]"} rounded-xl border transition-all hover:translate-x-2 hover:shadow-[0_4px_15px_rgba(102,126,234,0.12)]`}
            >
              <div className="w-12 flex-shrink-0 text-center text-2xl">
                {index === 0 ? "🥇" : index === 1 ? "🥈" : index === 2 ? "🥉" : `${index + 1}.`}
              </div>
              <div className="min-w-0 flex-1">
                <div className={`font-semibold ${themeClasses.text} truncate text-sm md:text-base`}>
                  {algorithm.name}
                </div>
                <div
                  className={`h-3 ${theme === "dark" ? "bg-gray-700" : "bg-gray-200"} mt-2 overflow-hidden rounded-full`}
                >
                  <div
                    className="h-full rounded-full transition-all"
                    style={{
                      width: `${(algorithm.avgTime / maxTime) * 100}%`,
                      background:
                        index === 0
                          ? "linear-gradient(90deg, #10b981, #059669)"
                          : index === 1
                            ? "linear-gradient(90deg, #3b82f6, #2563eb)"
                            : index === 2
                              ? "linear-gradient(90deg, #f59e0b, #d97706)"
                              : "linear-gradient(90deg, #667eea, #764ba2)"
                    }}
                  />
                </div>
              </div>
              <div className="flex-shrink-0 text-right">
                <div className={`text-lg font-bold md:text-xl ${themeClasses.text}`}>
                  {algorithm.avgTime.toFixed(2)}ms
                </div>
                <div className={`text-xs ${themeClasses.textMuted}`}>
                  ({algorithm.minTime.toFixed(1)}-{algorithm.maxTime.toFixed(1)}ms)
                </div>
              </div>
            </div>
          ))}
        </div>
      </div>
    </div>
  );
};

type AlgorithmStatsCardProps = {
  container: ImageContainer;
};

export const AlgorithmStatsCard: React.FC<AlgorithmStatsCardProps> = ({ container }) => {
  const { theme, getThemeClasses } = useTheme();
  const themeClasses = getThemeClasses();
  const info = container.algorithmInfo;
  if (!info) {
    return null;
  }

  const images = container.images;
  const iterations = images.reduce((sum, img) => sum + (img.metrics?.iterations ?? 0), 0);
  const avgIterations = iterations / images.length;

  return (
    <div
      className={`${themeClasses.bgSecondary}/80 rounded-lg border p-4 shadow-md backdrop-blur-sm ${themeClasses.border}`}
    >
      <div className="flex flex-wrap gap-3 md:gap-4">
        {info.avgTime && (
          <div
            className={`flex items-center gap-2 px-3 py-2 ${theme === "dark" ? "bg-blue-900/30" : "bg-blue-100"} rounded-lg`}
          >
            <span className="text-xl">⏱️</span>
            <span className={`text-sm ${themeClasses.textSecondary}`}>
              <strong>{info.avgTime.toFixed(2)}ms</strong> avg
            </span>
          </div>
        )}

        {info.minTime && info.maxTime && (
          <div
            className={`flex items-center gap-2 px-3 py-2 ${theme === "dark" ? "bg-purple-900/30" : "bg-purple-100"} rounded-lg`}
          >
            <span className="text-xl">📊</span>
            <span className={`text-sm ${themeClasses.textSecondary}`}>
              {info.minTime.toFixed(2)}ms - {info.maxTime.toFixed(2)}ms
            </span>
          </div>
        )}

        {avgIterations > 0 && (
          <div
            className={`flex items-center gap-2 px-3 py-2 ${theme === "dark" ? "bg-green-900/30" : "bg-green-100"} rounded-lg`}
          >
            <span className="text-xl">🔄</span>
            <span className={`text-sm ${themeClasses.textSecondary}`}>
              <strong>{Math.round(avgIterations).toLocaleString()}</strong> iterations
            </span>
          </div>
        )}

        {info.throughput && (
          <div
            className={`flex items-center gap-2 px-3 py-2 ${theme === "dark" ? "bg-yellow-900/30" : "bg-yellow-100"} rounded-lg`}
          >
            <span className="text-xl">⚡</span>
            <span className={`text-sm ${themeClasses.textSecondary}`}>
              <strong>{info.throughput.toFixed(1)}</strong> img/s
            </span>
          </div>
        )}

        {info.complexity && (
          <div className="flex items-center gap-2 rounded-lg bg-orange-100 px-3 py-2 dark:bg-orange-900/30">
            <span className="text-xl">📐</span>
            <span className="text-sm text-gray-700 dark:text-gray-300">
              Complexity: <strong>{info.complexity}</strong>
            </span>
          </div>
        )}
      </div>

      {info.stdDev && (
        <div className="mt-3 flex flex-wrap gap-3">
          <div className="flex items-center gap-2 rounded-lg bg-gray-100 px-3 py-2 dark:bg-gray-700">
            <span className="text-xl">📈</span>
            <span className="text-sm text-gray-600 dark:text-gray-400">StdDev: {info.stdDev.toFixed(3)}ms</span>
          </div>
        </div>
      )}
    </div>
  );
};

type ImageMetricsTooltipProps = {
  image: ImageData;
};

export const ImageMetricsTooltip: React.FC<ImageMetricsTooltipProps> = ({ image }) => {
  const m = image.metrics;
  if (!m) {
    return null;
  }

  return (
    <div className="max-w-xs rounded-xl border border-gray-200 bg-white p-4 shadow-2xl dark:border-gray-700 dark:bg-gray-800">
      <div className="mb-3 border-b border-gray-200 pb-2 text-lg font-bold text-gray-800 dark:border-gray-700 dark:text-white">
        {image.label}
      </div>
      <div className="space-y-2">
        {m.iterations && (
          <div className="flex items-center justify-between text-sm">
            <span className="text-gray-600 dark:text-gray-400">Iterations:</span>
            <span className="font-semibold text-gray-800 dark:text-white">{m.iterations.toLocaleString()}</span>
          </div>
        )}

        {m.cpuTime && (
          <div className="flex items-center justify-between text-sm">
            <span className="text-gray-600 dark:text-gray-400">CPU Time:</span>
            <span className="font-semibold text-gray-800 dark:text-white">{(m.cpuTime / 1000000).toFixed(3)}ms</span>
          </div>
        )}

        {m.realTime && (
          <div className="flex items-center justify-between text-sm">
            <span className="text-gray-600 dark:text-gray-400">Real Time:</span>
            <span className="font-semibold text-gray-800 dark:text-white">{(m.realTime / 1000000).toFixed(3)}ms</span>
          </div>
        )}

        {m.executionTimeMs && (
          <div className="flex items-center justify-between text-sm">
            <span className="text-gray-600 dark:text-gray-400">Execution:</span>
            <span className="font-semibold text-gray-800 dark:text-white">{m.executionTimeMs.toFixed(3)}ms</span>
          </div>
        )}

        {m.memoryUsageMB && (
          <div className="flex items-center justify-between text-sm">
            <span className="text-gray-600 dark:text-gray-400">Memory:</span>
            <span className="font-semibold text-gray-800 dark:text-white">{m.memoryUsageMB.toFixed(2)}MB</span>
          </div>
        )}

        {m.itemsPerSecond && (
          <div className="flex items-center justify-between text-sm">
            <span className="text-gray-600 dark:text-gray-400">Throughput:</span>
            <span className="font-semibold text-gray-800 dark:text-white">{m.itemsPerSecond.toFixed(1)} items/s</span>
          </div>
        )}

        {m.nonZeroPixels && m.pixelCount && (
          <div className="flex items-center justify-between text-sm">
            <span className="text-gray-600 dark:text-gray-400">Fill Ratio:</span>
            <span className="font-semibold text-gray-800 dark:text-white">
              {((m.nonZeroPixels / m.pixelCount) * 100).toFixed(1)}%
            </span>
          </div>
        )}
      </div>
    </div>
  );
};
