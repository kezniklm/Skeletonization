import type { AlgorithmAvgTime } from "../utils";
import { useTheme } from "../../../contexts/ThemeContext";

type PerformanceComparisonProps = {
  fastest: AlgorithmAvgTime;
  slowest: AlgorithmAvgTime;
};

export const PerformanceComparison = ({ fastest, slowest }: PerformanceComparisonProps) => {
  const { theme, getThemeClasses } = useTheme();
  const themeClasses = getThemeClasses();

  return (
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
          <div className={`text-2xl font-bold md:text-3xl ${theme === "dark" ? "text-red-400" : "text-red-600"} mb-4`}>
            {slowest.avgTime.toFixed(2)}ms
          </div>
          <div className={`h-3 ${theme === "dark" ? "bg-gray-700" : "bg-gray-200"} mb-2 overflow-hidden rounded-full`}>
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
  );
};
