/**
 * @file AlgorithmRankings.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Displays ranked algorithm performance list.
 * @description Visualizes average execution time ordering with rank badges and progress bars.
 * @version 1.0
 * @date 2026-03-16
 */

import type { AlgorithmAvgTime } from "../utils";
import { useTheme } from "../../../contexts/ThemeContext";

type AlgorithmRankingsProps = {
  algorithmAvgTimes: AlgorithmAvgTime[];
  maxTime: number;
};

/**
 * @brief Returns rank icon for leaderboard position.
 * @param index Zero-based ranking index.
 * @returns Emoji or numeric rank label.
 */
const getRankIcon = (index: number): string => {
  if (index === 0) {
    return "🥇";
  }
  if (index === 1) {
    return "🥈";
  }
  if (index === 2) {
    return "🥉";
  }
  return `${index + 1}.`;
};

/**
 * @brief Returns progress bar gradient based on ranking position.
 * @param index Zero-based ranking index.
 * @returns CSS linear-gradient string.
 */
const getProgressGradient = (index: number): string => {
  if (index === 0) {
    return "linear-gradient(90deg, #10b981, #059669)";
  }
  if (index === 1) {
    return "linear-gradient(90deg, #3b82f6, #2563eb)";
  }
  if (index === 2) {
    return "linear-gradient(90deg, #f59e0b, #d97706)";
  }
  return "linear-gradient(90deg, #667eea, #764ba2)";
};

/**
 * @brief Renders ranked algorithm cards by average execution time.
 * @param algorithmAvgTimes Ordered list of algorithm timing summaries.
 * @param maxTime Maximum average time used for bar normalization.
 * @returns Rankings panel JSX.
 */
export const AlgorithmRankings = ({ algorithmAvgTimes, maxTime }: AlgorithmRankingsProps) => {
  const { theme, getThemeClasses } = useTheme();
  const themeClasses = getThemeClasses();

  return (
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
            <div className={`w-12 flex-shrink-0 text-center text-2xl ${themeClasses.text}`}>{getRankIcon(index)}</div>
            <div className="min-w-0 flex-1">
              <div className={`font-semibold ${themeClasses.text} truncate text-sm md:text-base`}>{algorithm.name}</div>
              <div
                className={`h-3 ${theme === "dark" ? "bg-gray-700" : "bg-gray-200"} mt-2 overflow-hidden rounded-full`}
              >
                <div
                  className="h-full rounded-full transition-all"
                  style={{
                    width: `${(algorithm.avgTime / maxTime) * 100}%`,
                    background: getProgressGradient(index)
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
  );
};
