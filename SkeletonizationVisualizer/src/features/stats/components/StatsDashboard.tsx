import type { BenchmarkData } from "../../../types";
import { useTheme } from "../../../contexts/ThemeContext";
import { useBenchmarkStats } from "../hooks";

import { AlgorithmRankings } from "./AlgorithmRankings";
import { PerformanceComparison } from "./PerformanceComparison";
import { StatCard } from "./StatCard";

type StatsDashboardProps = {
  data: BenchmarkData;
};

export const StatsDashboard = ({ data }: StatsDashboardProps) => {
  const { getThemeClasses } = useTheme();
  const themeClasses = getThemeClasses();
  const stats = useBenchmarkStats(data);

  return (
    <div className="mx-auto max-w-7xl px-4 py-6 sm:px-6 md:py-8 lg:px-8">
      <h2 className={`text-2xl font-bold md:text-3xl ${themeClasses.text} mb-6 flex items-center gap-3`}>
        📊 Benchmark Overview
      </h2>

      <div className="mb-8 grid grid-cols-1 gap-4 sm:grid-cols-2 md:gap-6 lg:grid-cols-4">
        <StatCard
          icon="🎯"
          value={stats.totalAlgorithms}
          label="Algorithms Tested"
          gradient="from-purple-500 to-indigo-600"
        />
        <StatCard
          icon="🖼️"
          value={stats.totalTestImages}
          label="Test Images"
          gradient="from-green-500 to-emerald-600"
        />
        <StatCard
          icon="⏱️"
          value={`${stats.avgTime.toFixed(2)}ms`}
          label="Average Time"
          subLabel={`${stats.minTime.toFixed(2)}ms - ${stats.maxTime.toFixed(2)}ms`}
          gradient="from-blue-500 to-cyan-600"
        />
        <StatCard
          icon="🔄"
          value={stats.totalIterations.toLocaleString()}
          label="Total Iterations"
          gradient="from-orange-500 to-red-600"
        />
      </div>

      {stats.fastest && stats.slowest && <PerformanceComparison fastest={stats.fastest} slowest={stats.slowest} />}

      <AlgorithmRankings algorithmAvgTimes={stats.algorithmAvgTimes} maxTime={stats.maxTime} />
    </div>
  );
};
