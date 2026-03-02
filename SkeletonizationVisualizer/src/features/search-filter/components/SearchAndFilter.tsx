import { useState, useEffect, useMemo } from "react";

import { useTheme } from "../../../contexts/ThemeContext";
import type { ImageContainer } from "../../../types";

type SearchAndFilterProps = {
  containers: ImageContainer[];
  onFiltersChange: (filteredContainers: ImageContainer[]) => void;
};

export const SearchAndFilter = ({ containers, onFiltersChange }: SearchAndFilterProps) => {
  const { getThemeClasses } = useTheme();
  const themeClasses = getThemeClasses();

  // Internal state management
  const [searchTerm, setSearchTerm] = useState("");
  const [selectedAlgorithm, setSelectedAlgorithm] = useState<string>("all");

  // Filter logic
  const getAlgorithmList = () => ["all", ...containers.map((c) => c.name)];

  const filteredContainers = useMemo(
    () =>
      containers.filter((container) => {
        const matchesAlgorithm = selectedAlgorithm === "all" || container.name === selectedAlgorithm;
        const matchesSearch =
          searchTerm === "" ||
          container.name.toLowerCase().includes(searchTerm.toLowerCase()) ||
          container.images.some((img) => img.label.toLowerCase().includes(searchTerm.toLowerCase()));
        return matchesAlgorithm && matchesSearch;
      }),
    [containers, selectedAlgorithm, searchTerm]
  );

  // Notify parent of filter changes
  useEffect(() => {
    onFiltersChange(filteredContainers);
  }, [filteredContainers, onFiltersChange]);

  return (
    <section className="mx-auto max-w-7xl animate-[fadeInUp_0.6s_ease-out] px-4 py-6 sm:px-6 md:py-8 lg:px-8">
      <div className={`${themeClasses.bgSecondary} rounded-xl border shadow-md ${themeClasses.border} p-4 md:p-6`}>
        <div className="mb-6 flex flex-col gap-4 md:flex-row md:items-center md:justify-between">
          <h2 className={`text-2xl font-bold md:text-3xl ${themeClasses.text} flex items-center gap-3`}>
            🔍 Browse Results
          </h2>
          <div className={`flex items-center gap-2 text-sm md:gap-4 md:text-base ${themeClasses.textMuted}`}>
            <span className="rounded-full bg-[#667eea]/10 px-3 py-1 font-medium text-[#667eea]">
              {filteredContainers?.length || 0} benchmarks
            </span>
            <span className={themeClasses.textMuted}>•</span>
            <span className="rounded-full bg-[#764ba2]/10 px-3 py-1 font-medium text-[#764ba2]">
              {filteredContainers?.reduce((sum, c) => sum + c.images.length, 0) || 0} images
            </span>
          </div>
        </div>
        <div className="flex flex-col gap-4 md:flex-row">
          <div className="relative flex-1">
            <span className={`absolute top-1/2 left-4 -translate-y-1/2 ${themeClasses.textMuted} text-lg`}>🔍</span>
            <input
              type="text"
              className={`w-full rounded-lg py-3 pr-12 pl-12 ${themeClasses.bgTertiary} border ${themeClasses.borderMuted} ${themeClasses.text} placeholder-gray-500 transition-all focus:border-[#667eea] focus:ring-2 focus:ring-[#667eea] focus:outline-none`}
              placeholder="Search benchmarks, images, or descriptions..."
              value={searchTerm}
              onChange={(e) => setSearchTerm(e.target.value)}
            />
            {searchTerm && (
              <button
                className={`absolute top-1/2 right-4 -translate-y-1/2 ${themeClasses.textMuted} hover:${themeClasses.textSecondary} transition-colors`}
                onClick={() => setSearchTerm("")}
                title="Clear search"
              >
                ✕
              </button>
            )}
          </div>
          <select
            className={`rounded-lg px-4 py-3 ${themeClasses.bgTertiary} border ${themeClasses.borderMuted} ${themeClasses.text} min-w-[200px] cursor-pointer transition-all focus:border-[#667eea] focus:ring-2 focus:ring-[#667eea] focus:outline-none`}
            value={selectedAlgorithm}
            onChange={(e) => setSelectedAlgorithm(e.target.value)}
          >
            {getAlgorithmList().map((algo) => (
              <option key={algo} value={algo}>
                {algo === "all" ? "📁 All Benchmarks" : `🔬 ${algo}`}
              </option>
            ))}
          </select>
        </div>
      </div>
    </section>
  );
};
