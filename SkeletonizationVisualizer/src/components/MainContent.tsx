import { useTheme } from "../contexts/ThemeContext";
import type { ImageContainer, ImageData } from "../types";

import { ImageGallery } from "./ImageGallery";

type MainContentProps = {
  filteredContainers: ImageContainer[];
  onImageClick: (image: ImageData, container: ImageContainer) => void;
  onDownload: (image: ImageData) => void;
  onCompare: (original: ImageData, processed: ImageData) => void;
};

export const MainContent = ({ filteredContainers, onImageClick, onDownload, onCompare }: MainContentProps) => {
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
          <div
            className={`mb-6 p-4 md:p-6 ${themeClasses.bgSecondary} rounded-xl border shadow-md ${themeClasses.border}`}
          >
            <h2 className={`text-2xl font-bold md:text-3xl ${themeClasses.text} mb-3`}>{container.name}</h2>
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
          <ImageGallery
            container={container}
            onImageClick={onImageClick}
            onDownload={onDownload}
            onCompare={onCompare}
          />
        </section>
      ))}
    </main>
  );
};
