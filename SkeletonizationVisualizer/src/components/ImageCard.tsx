import { useTheme } from "../contexts/ThemeContext";
import type { ImageData, ImageContainer } from "../types";

type ImageCardProps = {
  image: ImageData;
  container: ImageContainer;
  index: number;
  onImageClick: (image: ImageData, container: ImageContainer) => void;
  onDownload: (image: ImageData) => void;
  onCompare: (original: ImageData, processed: ImageData) => void;
};

export const ImageCard = ({ image, container, index, onImageClick, onDownload, onCompare }: ImageCardProps) => {
  const { theme, getThemeClasses } = useTheme();
  const themeClasses = getThemeClasses();

  return (
    <div
      key={image.id}
      className={`group ${themeClasses.bgSecondary} overflow-hidden rounded-xl shadow-lg transition-all duration-300 hover:-translate-y-2 hover:shadow-[0_12px_24px_rgba(102,126,234,0.3)] ${
        index === 0 ? "border-2 border-[#f59e0b]" : `border ${themeClasses.border}`
      }`}
    >
      <button
        type="button"
        aria-label={`View ${image.label}`}
        className={`relative aspect-square cursor-pointer overflow-hidden ${themeClasses.bgTertiary} focus-visible:ring-2 focus-visible:ring-[#667eea] focus-visible:ring-offset-2 focus-visible:outline-none`}
        onClick={() => onImageClick(image, container)}
      >
        <img
          src={`data:image/png;base64,${image.data}`}
          alt={image.label}
          className="h-full w-full object-contain transition-transform duration-300 group-hover:scale-110"
          loading="lazy"
        />
        <div className="absolute inset-0 flex items-center justify-center bg-gradient-to-t from-black/60 via-black/20 to-transparent opacity-0 transition-opacity duration-300 group-hover:opacity-100">
          <span className="scale-0 transform text-5xl transition-transform duration-300 group-hover:scale-100">🔍</span>
        </div>
      </button>
      <div className="p-4">
        <p className={`text-sm font-semibold md:text-base ${themeClasses.text} mb-3 truncate`} title={image.label}>
          {image.label}
        </p>
        {index === 0 ? (
          <div className="mb-3 flex flex-wrap gap-2">
            <span className="rounded-md bg-[#f59e0b] px-2 py-1 text-xs font-bold text-white">📄 Original</span>
          </div>
        ) : (
          image.metrics && (
            <div className="mb-3 flex flex-wrap gap-2">
              {image.metrics.executionTimeMs && (
                <span
                  className={`rounded-md px-2 py-1 ${theme === "dark" ? "bg-blue-900/30 text-blue-300" : "bg-blue-100 text-blue-700"} text-xs font-medium`}
                >
                  ⏱️ {image.metrics.executionTimeMs.toFixed(2)}ms
                </span>
              )}
              {image.metrics.nonZeroPixels && (
                <span
                  className={`rounded-md px-2 py-1 ${theme === "dark" ? "bg-purple-900/30 text-purple-300" : "bg-purple-100 text-purple-700"} text-xs font-medium`}
                >
                  📊 {image.metrics.nonZeroPixels} px
                </span>
              )}
            </div>
          )
        )}
        <div className="mb-2 grid grid-cols-6 gap-2">
          <button
            className={`flex h-9 items-center justify-center rounded-lg bg-[#667eea] px-3 py-2 text-xs font-medium whitespace-nowrap text-white shadow-sm transition-all hover:-translate-y-0.5 hover:shadow-[0_4px_12px_rgba(102,126,234,0.4)] ${index === 0 ? "col-span-3" : "col-span-2"}`}
            onClick={(e) => {
              e.stopPropagation();
              onImageClick(image, container);
            }}
            title="View Image"
          >
            🔍 View
          </button>
          <button
            className={`flex h-9 items-center justify-center rounded-lg bg-[#10b981] px-3 py-2 text-xs font-medium whitespace-nowrap text-white shadow-sm transition-all hover:-translate-y-0.5 hover:shadow-[0_4px_12px_rgba(16,185,129,0.4)] ${index === 0 ? "col-span-3" : "col-span-2"}`}
            onClick={(e) => {
              e.stopPropagation();
              onDownload(image);
            }}
            title="Download Image"
          >
            💾 Save
          </button>
          {index > 0 && (
            <button
              className="col-span-2 flex h-9 items-center justify-center rounded-lg bg-[#764ba2] px-3 py-2 text-xs font-medium whitespace-nowrap text-white shadow-sm transition-all hover:-translate-y-0.5 hover:shadow-[0_4px_12px_rgba(118,75,162,0.4)]"
              onClick={(e) => {
                e.stopPropagation();
                onCompare(container.images[0], image);
              }}
              title="Compare with Original"
            >
              🔀 Compare
            </button>
          )}
        </div>
        <p className={`text-xs ${themeClasses.textMuted} text-center`}>
          {image.width} × {image.height}px
        </p>
      </div>
    </div>
  );
};
