import { useEffect, useRef, useState } from "react";

import { useTheme } from "../../contexts/ThemeContext";
import type { ImageData, ComparisonMode } from "../../types";

type ComparisonModalProps = {
  original: ImageData | null;
  processedImages: ImageData[];
  isOpen: boolean;
  onClose: () => void;
  comparisonMode: ComparisonMode;
  onComparisonModeChange: (mode: ComparisonMode) => void;
  sliderPosition: number;
  onSliderPositionChange: (position: number) => void;
};

export const ComparisonModal = ({
  original,
  processedImages,
  isOpen,
  onClose,
  comparisonMode,
  onComparisonModeChange,
  sliderPosition,
  onSliderPositionChange
}: ComparisonModalProps) => {
  const { theme, getThemeClasses } = useTheme();
  const themeClasses = getThemeClasses();

  const dialogRef = useRef<HTMLDivElement>(null);
  const dragAreaRef = useRef<HTMLDivElement>(null);

  const [activeProcessedIndex, setActiveProcessedIndex] = useState(0);
  const [overlayOpacity, setOverlayOpacity] = useState(50);

  const headerId = "comparison-modal-title";
  const descId = "comparison-modal-desc";

  useEffect(() => {
    if (!isOpen) {
      return;
    }

    dialogRef.current?.focus();

    const onKey = (e: KeyboardEvent) => {
      if (e.key === "Escape") {
        e.stopPropagation();
        onClose();
      }
    };

    document.addEventListener("keydown", onKey, { capture: true });
    return () => document.removeEventListener("keydown", onKey, { capture: true });
  }, [isOpen, onClose]);

  useEffect(() => {
    if (!isOpen) {
      return;
    }

    setActiveProcessedIndex(0);
    setOverlayOpacity(50);
  }, [isOpen, processedImages]);

  const activeProcessed = processedImages[activeProcessedIndex] ?? processedImages[0] ?? null;

  if (!isOpen || !original || !activeProcessed) {
    return null;
  }

  const handleBackdropClick = (e: React.MouseEvent<HTMLDivElement>) => {
    if (e.target === e.currentTarget) {
      onClose();
    }
  };

  const clamp = (value: number) => Math.max(0, Math.min(100, value));

  const updateFromClientX = (clientX: number) => {
    const rect = dragAreaRef.current?.getBoundingClientRect();
    if (!rect) {
      return;
    }

    const x = clientX - rect.left;
    onSliderPositionChange(clamp((x / rect.width) * 100));
  };

  const handleMouseDown = (e: React.MouseEvent<HTMLDivElement>) => {
    updateFromClientX(e.clientX);

    const move = (me: MouseEvent) => updateFromClientX(me.clientX);
    const up = () => {
      document.removeEventListener("mousemove", move);
      document.removeEventListener("mouseup", up);
    };

    document.addEventListener("mousemove", move);
    document.addEventListener("mouseup", up);
  };

  const handleTouchStart = (e: React.TouchEvent<HTMLDivElement>) => {
    if (e.touches.length) {
      updateFromClientX(e.touches[0].clientX);
    }
  };

  const handleTouchMove = (e: React.TouchEvent<HTMLDivElement>) => {
    if (e.touches.length) {
      updateFromClientX(e.touches[0].clientX);
    }
  };

  const handleDragAreaKeyDown = (e: React.KeyboardEvent<HTMLDivElement>) => {
    const step = (amount: number) => onSliderPositionChange(clamp(sliderPosition + amount));

    switch (e.key) {
      case "ArrowLeft":
        e.preventDefault();
        step(-1);
        break;
      case "ArrowRight":
        e.preventDefault();
        step(1);
        break;
      case "PageDown":
        e.preventDefault();
        step(-10);
        break;
      case "PageUp":
        e.preventDefault();
        step(10);
        break;
      case "Home":
        e.preventDefault();
        onSliderPositionChange(0);
        break;
      case "End":
        e.preventDefault();
        onSliderPositionChange(100);
        break;
      default:
        break;
    }
  };

  const sideBySideImages = [original, ...processedImages];
  const sideBySideCount = sideBySideImages.length;
  const isSmallSideBySide = sideBySideCount <= 3;

  const sideBySideCardWidth =
    sideBySideCount === 2 ? "min(700px, 90vw)" : sideBySideCount === 3 ? "min(560px, 85vw)" : "min(500px, 80vw)";

  return (
    <div
      className="fixed inset-0 z-50 flex animate-[fadeIn_0.2s_ease-out] items-center justify-center bg-black/80 backdrop-blur-sm"
      onClick={handleBackdropClick}
      aria-hidden="true"
    >
      <div
        ref={dialogRef}
        role="dialog"
        aria-modal="true"
        aria-labelledby={headerId}
        aria-describedby={descId}
        tabIndex={-1}
        className={`h-full max-h-[95vh] w-full max-w-[95vw] ${themeClasses.bgSecondary} flex animate-[scaleIn_0.3s_cubic-bezier(0.4,0,0.2,1)] flex-col overflow-hidden rounded-2xl shadow-2xl outline-none`}
      >
        <div
          className={`flex flex-col gap-4 p-4 md:p-6 ${
            theme === "dark"
              ? "bg-gradient-to-r from-gray-800 to-gray-800"
              : "bg-gradient-to-r from-purple-50 to-pink-50"
          } border-b ${themeClasses.border}`}
        >
          <div className="flex flex-col gap-3 md:flex-row md:items-center md:justify-between">
            <div>
              <h3 id={headerId} className={`text-lg font-bold md:text-2xl ${themeClasses.text}`}>
                Skeleton Comparison
              </h3>
              <p id={descId} className="sr-only">
                Compare original image against one or more processed skeletons using side-by-side, slider, or overlay
                modes.
              </p>
            </div>
            <div className="flex items-center gap-2">
              <span className={`rounded-lg px-3 py-2 text-sm ${themeClasses.bgTertiary} ${themeClasses.textSecondary}`}>
                {processedImages.length} selected
              </span>
              <button
                type="button"
                className="rounded-lg bg-red-500 px-3 py-2 font-bold text-white transition-colors hover:bg-red-600"
                onClick={onClose}
                aria-label="Close comparison modal"
              >
                ✕
              </button>
            </div>
          </div>

          <div className="flex flex-wrap items-center gap-2">
            <button
              type="button"
              aria-pressed={comparisonMode === "side-by-side"}
              className={`rounded-lg px-4 py-2 text-sm font-medium transition-all ${
                comparisonMode === "side-by-side"
                  ? "bg-gradient-to-r from-purple-500 to-pink-500 text-white shadow-lg"
                  : `${themeClasses.bgTertiary} ${themeClasses.textSecondary} hover:${themeClasses.bgTertiary}`
              }`}
              onClick={() => onComparisonModeChange("side-by-side")}
            >
              ⬌ Side-by-Side
            </button>
            <button
              type="button"
              aria-pressed={comparisonMode === "slider"}
              className={`rounded-lg px-4 py-2 text-sm font-medium transition-all ${
                comparisonMode === "slider"
                  ? "bg-gradient-to-r from-purple-500 to-pink-500 text-white shadow-lg"
                  : `${themeClasses.bgTertiary} ${themeClasses.textSecondary} hover:${themeClasses.bgTertiary}`
              }`}
              onClick={() => onComparisonModeChange("slider")}
            >
              ⬍ Slider
            </button>
            <button
              type="button"
              aria-pressed={comparisonMode === "overlay"}
              className={`rounded-lg px-4 py-2 text-sm font-medium transition-all ${
                comparisonMode === "overlay"
                  ? "bg-gradient-to-r from-purple-500 to-pink-500 text-white shadow-lg"
                  : `${themeClasses.bgTertiary} ${themeClasses.textSecondary} hover:${themeClasses.bgTertiary}`
              }`}
              onClick={() => onComparisonModeChange("overlay")}
            >
              🎭 Overlay
            </button>
          </div>

          {processedImages.length > 1 && comparisonMode !== "side-by-side" && (
            <div className="flex flex-wrap items-center gap-2">
              {processedImages.map((image, index) => (
                <button
                  key={image.id}
                  type="button"
                  aria-pressed={activeProcessedIndex === index}
                  onClick={() => setActiveProcessedIndex(index)}
                  className={`rounded-lg px-3 py-2 text-sm font-medium transition-all ${
                    activeProcessedIndex === index
                      ? "bg-[#764ba2] text-white"
                      : `${themeClasses.bgTertiary} ${themeClasses.textSecondary}`
                  }`}
                >
                  {image.label}
                </button>
              ))}
            </div>
          )}
        </div>

        <div className={`flex-1 overflow-auto p-4 md:p-6 ${themeClasses.bg}`}>
          {comparisonMode === "side-by-side" && (
            <div
              className={`comparison-scrollbar flex h-full gap-4 overflow-x-auto pb-4 md:gap-6 ${
                isSmallSideBySide ? "justify-center" : ""
              }`}
            >
              {sideBySideImages.map((image) => (
                <figure
                  key={image.id}
                  className={`relative flex-shrink-0 ${themeClasses.bgSecondary} overflow-hidden rounded-xl border shadow-lg ${themeClasses.border}`}
                  style={{ width: sideBySideCardWidth, height: "100%" }}
                >
                  <img
                    src={`data:image/png;base64,${image.data}`}
                    alt={image.label}
                    className="h-full w-full object-contain"
                  />
                  <figcaption className="absolute right-0 bottom-0 left-0 bg-gradient-to-t from-black/70 to-transparent p-4">
                    <span className="text-sm font-semibold text-white md:text-base">{image.label}</span>
                  </figcaption>
                </figure>
              ))}
            </div>
          )}

          {comparisonMode === "slider" && (
            <div className="flex h-full flex-col gap-4">
              <div
                ref={dragAreaRef}
                role="slider"
                aria-label="Comparison slider"
                aria-valuemin={0}
                aria-valuemax={100}
                aria-valuenow={Math.round(sliderPosition)}
                tabIndex={0}
                className={`relative flex-1 ${themeClasses.bgSecondary} overflow-hidden rounded-xl border shadow-lg ${themeClasses.border} cursor-col-resize outline-none`}
                onMouseDown={handleMouseDown}
                onTouchStart={handleTouchStart}
                onTouchMove={handleTouchMove}
                onKeyDown={handleDragAreaKeyDown}
              >
                <img
                  src={`data:image/png;base64,${original.data}`}
                  alt={original.label}
                  className="pointer-events-none h-full w-full object-contain select-none"
                  draggable="false"
                />
                <div
                  className="pointer-events-none absolute inset-0 overflow-hidden"
                  style={{ clipPath: `inset(0 ${100 - sliderPosition}% 0 0)` }}
                >
                  <img
                    src={`data:image/png;base64,${activeProcessed.data}`}
                    alt={activeProcessed.label}
                    className="h-full w-full object-contain select-none"
                    draggable="false"
                  />
                </div>
                <div
                  className="bg-accent pointer-events-none absolute top-0 bottom-0 w-1 shadow-lg"
                  style={{ left: `${sliderPosition}%`, transform: "translateX(-50%)" }}
                  aria-hidden="true"
                >
                  <div className="bg-accent absolute top-1/2 left-1/2 flex h-10 w-10 -translate-x-1/2 -translate-y-1/2 items-center justify-center rounded-full text-lg font-bold text-white shadow-xl">
                    ⬌
                  </div>
                </div>
              </div>

              <label className="sr-only" htmlFor="comparison-range">
                Adjust comparison slider
              </label>
              <input
                id="comparison-range"
                type="range"
                min={0}
                max={100}
                value={sliderPosition}
                onChange={(e) => onSliderPositionChange(Number(e.target.value))}
                className={`h-2 w-full ${theme === "dark" ? "bg-gray-700" : "bg-gray-300"} accent-accent cursor-pointer appearance-none rounded-lg`}
              />
              <div className={`flex justify-between text-sm ${themeClasses.textMuted} font-medium`}>
                <span>{original.label}</span>
                <span>{activeProcessed.label}</span>
              </div>
            </div>
          )}

          {comparisonMode === "overlay" && (
            <div className="flex h-full flex-col gap-4">
              <div
                className={`relative flex-1 ${themeClasses.bgSecondary} overflow-hidden rounded-xl border shadow-lg ${themeClasses.border}`}
              >
                <img
                  src={`data:image/png;base64,${original.data}`}
                  alt={original.label}
                  className="absolute inset-0 h-full w-full object-contain"
                />
                <img
                  src={`data:image/png;base64,${activeProcessed.data}`}
                  alt={activeProcessed.label}
                  className="absolute inset-0 h-full w-full object-contain"
                  style={{ opacity: overlayOpacity / 100 }}
                />
              </div>
              <label className="sr-only" htmlFor="overlay-opacity">
                Adjust overlay opacity
              </label>
              <input
                id="overlay-opacity"
                type="range"
                min={0}
                max={100}
                value={overlayOpacity}
                onChange={(e) => setOverlayOpacity(Number(e.target.value))}
                className="accent-accent h-2 w-full cursor-pointer appearance-none rounded-lg bg-gray-300 dark:bg-gray-700"
              />
              <div className={`flex justify-between text-sm ${themeClasses.textMuted} font-medium`}>
                <span>{original.label}</span>
                <span>{activeProcessed.label}</span>
              </div>
            </div>
          )}
        </div>
      </div>
    </div>
  );
};
