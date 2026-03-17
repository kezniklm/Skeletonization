/**
 * @file Navigation.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Renders top navigation for benchmark visualizer.
 * @description Displays title, theme toggle, export action, and data timestamp summary.
 * @version 1.0
 * @date 2026-03-16
 */

import { useTheme } from "../../../contexts/ThemeContext";
import type { BenchmarkData } from "../../../types";

type NavigationProps = {
  data: BenchmarkData;
  onToggleTheme: () => void;
  onExportAll: () => void;
};

/**
 * @brief Displays sticky navigation controls and metadata.
 * @param data Loaded benchmark dataset.
 * @param onToggleTheme Callback to toggle theme.
 * @param onExportAll Callback to export all images.
 * @returns Navigation bar JSX.
 */
export const Navigation = ({ data, onToggleTheme, onExportAll }: NavigationProps) => {
  const { theme, getThemeClasses } = useTheme();
  const themeClasses = getThemeClasses();

  return (
    <nav
      className={`sticky top-0 z-50 backdrop-blur-xl ${themeClasses.bgSecondary}/90 ${themeClasses.border} animate-[slideDown_0.4s_ease-out] border-b shadow-sm`}
    >
      <div className="mx-auto max-w-7xl px-4 py-4 sm:px-6 lg:px-8">
        <div className="flex items-center justify-between gap-4">
          <div className="flex items-center gap-4">
            <div className="flex-shrink-0 transition-transform duration-300 hover:scale-105">
              <svg width="32" height="32" viewBox="0 0 100 100" xmlns="http://www.w3.org/2000/svg">
                <defs>
                  <linearGradient id="iconGrad" x1="0%" y1="0%" x2="100%" y2="100%">
                    <stop offset="0%" style={{ stopColor: "#667eea" }} />
                    <stop offset="100%" style={{ stopColor: "#764ba2" }} />
                  </linearGradient>
                </defs>
                <rect x="44" y="20" width="12" height="60" rx="6" fill="url(#iconGrad)" />
                <rect x="30" y="20" width="40" height="12" rx="6" fill="url(#iconGrad)" />
                <rect x="30" y="68" width="40" height="12" rx="6" fill="url(#iconGrad)" />
                <circle cx="50" cy="35" r="7" fill="url(#iconGrad)" />
                <circle cx="50" cy="50" r="6" fill="url(#iconGrad)" />
                <circle cx="50" cy="65" r="7" fill="url(#iconGrad)" />
              </svg>
            </div>
            <div className="hidden sm:block">
              <h1 className="text-gradient text-xl font-bold md:text-2xl">Skeletonization Benchmark</h1>
              <p className={`text-xs md:text-sm ${themeClasses.textMuted}`}>Performance Analysis & Visualization</p>
            </div>
          </div>
          <div className="flex items-center gap-2 md:gap-3">
            <button
              className="flex items-center gap-2 rounded-lg bg-[#667eea] px-3 py-2 text-sm font-medium text-white shadow-sm transition-all hover:-translate-y-0.5 hover:shadow-[0_6px_20px_rgba(102,126,234,0.4)] md:px-4"
              onClick={onToggleTheme}
              title="Toggle Theme"
            >
              {theme === "light" ? "🌙 Dark" : "☀️ Light"}
            </button>
            <button
              className="flex items-center gap-2 rounded-lg bg-[#764ba2] px-3 py-2 text-sm font-medium text-white shadow-sm transition-all hover:-translate-y-0.5 hover:shadow-[0_6px_20px_rgba(118,75,162,0.4)] md:px-4"
              onClick={onExportAll}
              title="Export All Images"
            >
              📦 Export All
            </button>
          </div>
        </div>
      </div>
      <div className={`mx-auto max-w-7xl border-t px-4 py-2 sm:px-6 lg:px-8 ${themeClasses.border}/50`}>
        <span
          className={`inline-flex items-center gap-2 rounded-full px-3 py-1.5 ${themeClasses.bgTertiary} text-xs md:text-sm ${themeClasses.textSecondary}`}
        >
          📅 Last updated: {new Date(data.timestamp).toLocaleString()}
        </span>
      </div>
    </nav>
  );
};
