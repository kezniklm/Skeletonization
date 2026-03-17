/**
 * @file BenchmarkGallery.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Renders benchmark container sections with image galleries.
 * @description Displays per-container controls for multi-image comparison selection and delegates image rendering.
 * @version 1.0
 * @date 2026-03-16
 */

import { useTheme } from "../contexts/ThemeContext";
import { ImageGallery } from "../features/gallery/components";
import type { ImageContainer, ImageData } from "../types";

type BenchmarkGalleryProps = {
  filteredContainers: ImageContainer[];
  onImageClick: (image: ImageData, container: ImageContainer) => void;
  onDownload: (image: ImageData) => void;
  selectedComparisonIds: Record<string, string[]>;
  onToggleComparisonSelection: (containerName: string, image: ImageData) => void;
  onClearComparisonSelection: (containerName: string) => void;
  onSelectAllComparison: (container: ImageContainer) => void;
  onCompareSelected: (container: ImageContainer) => void;
};

/**
 * @brief Displays filtered benchmark containers and comparison actions.
 * @param filteredContainers Visible benchmark containers after filtering.
 * @param onImageClick Callback invoked when image preview is requested.
 * @param onDownload Callback invoked when image download is requested.
 * @param selectedComparisonIds Selected image IDs keyed by container name.
 * @param onToggleComparisonSelection Callback for toggling image comparison selection.
 * @param onClearComparisonSelection Callback for clearing container selection.
 * @param onSelectAllComparison Callback for selecting all processed images.
 * @param onCompareSelected Callback for opening multi-image comparison.
 * @returns Gallery section JSX.
 */
/** @brief Renders benchmark gallery sections and comparison controls. */
export const BenchmarkGallery = ({
  filteredContainers,
  onImageClick,
  onDownload,
  selectedComparisonIds,
  onToggleComparisonSelection,
  onClearComparisonSelection,
  onSelectAllComparison,
  onCompareSelected
}: BenchmarkGalleryProps) => {
  const { getThemeClasses } = useTheme();
  const themeClasses = getThemeClasses();

  if (!filteredContainers || filteredContainers.length === 0) {
    return (
      <div className="flex flex-col items-center justify-center py-20">
        <p className="text-lg text-gray-600 dark:text-gray-400">No images match your search criteria</p>
      </div>
    );
  }

  return (
    <main className="mx-auto max-w-7xl px-4 pb-12 sm:px-6 lg:px-8">
      {filteredContainers.map((container: ImageContainer, containerIndex: number) => (
        <section key={containerIndex} className="mb-8 animate-[fadeInUp_0.5s_ease-out] md:mb-12">
          {(() => {
            const selectedCount = selectedComparisonIds[container.name]?.length ?? 0;
            const selectableCount = Math.max(0, container.images.length - 1);
            const allSelected = selectableCount > 0 && selectedCount >= selectableCount;

            return (
              <div
                className={`mb-6 p-4 md:p-6 ${themeClasses.bgSecondary} rounded-xl border shadow-md ${themeClasses.border}`}
              >
                <div className="mb-3 flex flex-col gap-3 lg:flex-row lg:items-center lg:justify-between">
                  <h2 className={`text-2xl font-bold md:text-3xl ${themeClasses.text}`}>{container.name}</h2>
                  <div className="flex flex-wrap items-center gap-2">
                    <span
                      className={`rounded-lg px-3 py-2 text-sm font-medium ${themeClasses.bgTertiary} ${themeClasses.textSecondary}`}
                    >
                      {selectedCount} selected
                    </span>
                    <button
                      type="button"
                      onClick={() => onCompareSelected(container)}
                      disabled={selectedCount === 0}
                      className="rounded-lg bg-[#764ba2] px-4 py-2 text-sm font-medium text-white transition-all hover:-translate-y-0.5 hover:shadow-[0_4px_12px_rgba(118,75,162,0.4)] disabled:cursor-not-allowed disabled:opacity-50 disabled:hover:translate-y-0 disabled:hover:shadow-none"
                    >
                      🔀 Compare selected
                    </button>
                    <button
                      type="button"
                      onClick={() => onSelectAllComparison(container)}
                      disabled={selectableCount === 0 || allSelected}
                      className={`rounded-lg px-4 py-2 text-sm font-medium transition-all disabled:cursor-not-allowed disabled:opacity-50 ${themeClasses.bgTertiary} ${themeClasses.textSecondary}`}
                    >
                      Select all
                    </button>
                    <button
                      type="button"
                      onClick={() => onClearComparisonSelection(container.name)}
                      disabled={selectedCount === 0}
                      className={`rounded-lg px-4 py-2 text-sm font-medium transition-all disabled:cursor-not-allowed disabled:opacity-50 ${themeClasses.bgTertiary} ${themeClasses.textSecondary}`}
                    >
                      Clear
                    </button>
                  </div>
                </div>
                {container.algorithmInfo && (
                  <div className="flex flex-wrap gap-3">
                    {container.algorithmInfo.avgTime && (
                      <span className="rounded-lg border border-[#10b981]/30 bg-[#10b981]/10 px-4 py-2 text-sm font-medium text-[#10b981]">
                        ⏱️ Avg: {container.algorithmInfo.avgTime.toFixed(2)}ms
                      </span>
                    )}
                    {container.algorithmInfo.description && (
                      <span
                        className={`rounded-lg px-4 py-2 ${themeClasses.bgTertiary} ${themeClasses.textMuted} text-sm italic`}
                      >
                        {container.algorithmInfo.description}
                      </span>
                    )}
                  </div>
                )}
              </div>
            );
          })()}
          <ImageGallery
            container={container}
            onImageClick={onImageClick}
            onDownload={onDownload}
            selectedComparisonIds={selectedComparisonIds[container.name] ?? []}
            onToggleComparisonSelection={(image) => onToggleComparisonSelection(container.name, image)}
          />
        </section>
      ))}
    </main>
  );
};
