import { useRef, useEffect, useState } from "react";

import { useTheme } from "../../contexts/ThemeContext";
import type { ImageData, ImageContainer, ZoomState } from "../../types";

type ImageModalProps = {
  image: ImageData | null;
  container: ImageContainer | null;
  isOpen: boolean;
  onClose: () => void;
  onDownload: (image: ImageData) => void;
  onNavigate?: (direction: number) => void;
};

export const ImageModal = ({ image, container, isOpen, onClose, onDownload, onNavigate }: ImageModalProps) => {
  const { theme, getThemeClasses } = useTheme();
  const themeClasses = getThemeClasses();

  const dialogRef = useRef<HTMLDivElement>(null);
  const viewportRef = useRef<HTMLDivElement>(null);

  const headerId = "image-modal-title";
  const descId = "image-modal-desc";

  // Internal zoom/pan state management
  const [zoomState, setZoomState] = useState<ZoomState>({ scale: 1, translateX: 0, translateY: 0 });
  const [isDragging, setIsDragging] = useState(false);
  const [dragStart, setDragStart] = useState({ x: 0, y: 0 });

  // Reset zoom when image changes
  useEffect(() => {
    if (image) {
      setZoomState({ scale: 1, translateX: 0, translateY: 0 });
      setIsDragging(false);
    }
  }, [image?.id]);

  // Focus dialog & ESC support when opened
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

  // Zoom helpers
  const zoomIn = () => setZoomState((prev) => ({ ...prev, scale: Math.min(10, prev.scale * 1.2) }));
  const zoomOut = () => setZoomState((prev) => ({ ...prev, scale: Math.max(0.1, prev.scale / 1.2) }));
  const resetZoom = () => setZoomState({ scale: 1, translateX: 0, translateY: 0 });

  const fitToScreen = () => {
    if (!viewportRef.current || !image) {
      return;
    }
    const containerWidth = viewportRef.current.clientWidth;
    const containerHeight = viewportRef.current.clientHeight;
    const scaleX = containerWidth / image.width;
    const scaleY = containerHeight / image.height;
    const scale = Math.min(scaleX, scaleY, 1);
    setZoomState({ scale, translateX: 0, translateY: 0 });
  };

  const actualSize = () => setZoomState({ scale: 1, translateX: 0, translateY: 0 });

  // Keyboard shortcuts (global)
  useEffect(() => {
    if (!isOpen) {
      return;
    }

    const handleKeyPress = (e: KeyboardEvent) => {
      switch (e.key) {
        case "+":
        case "=":
          zoomIn();
          break;
        case "-":
        case "_":
          zoomOut();
          break;
        case "0":
          resetZoom();
          break;
        case "f":
        case "F":
          fitToScreen();
          break;
        case "d":
        case "D":
          if (image) {
            onDownload(image);
          }
          break;
        case "ArrowLeft":
          if (onNavigate) {
            onNavigate(-1);
          }
          break;
        case "ArrowRight":
          if (onNavigate) {
            onNavigate(1);
          }
          break;
        default:
          break;
      }
    };

    window.addEventListener("keydown", handleKeyPress);
    return () => window.removeEventListener("keydown", handleKeyPress);
  }, [isOpen, image, onDownload, onNavigate]);

  if (!isOpen || !image) {
    return null;
  }

  // Backdrop: close only if the click is on the backdrop itself
  const handleBackdropClick: React.MouseEventHandler<HTMLDivElement> = (e) => {
    if (e.target === e.currentTarget) {
      onClose();
    }
  };

  // --- Wheel & touch helpers (native events with passive: false) ---
  const handleWheelNative = (e: WheelEvent) => {
    if (!image) {
      return;
    }
    e.preventDefault();
    const delta = e.deltaY > 0 ? 0.9 : 1.1;
    setZoomState((prev) => {
      const newScale = Math.max(0.1, Math.min(10, prev.scale * delta));
      return { ...prev, scale: newScale };
    });
  };

  // Zoom and pan handlers (mouse)
  const handleMouseDown = (e: React.MouseEvent) => {
    if (!image || zoomState.scale === 1) {
      return;
    }
    setIsDragging(true);
    setDragStart({ x: e.clientX - zoomState.translateX, y: e.clientY - zoomState.translateY });
  };
  const handleMouseMove = (e: React.MouseEvent) => {
    if (!isDragging) {
      return;
    }
    setZoomState((prev) => ({
      ...prev,
      translateX: e.clientX - dragStart.x,
      translateY: e.clientY - dragStart.y
    }));
  };
  const handleMouseUp = () => setIsDragging(false);

  // Touch pan
  const touchStart = useRef<{ x: number; y: number } | null>(null);
  const handleTouchStartNative = (e: TouchEvent) => {
    if (!image || zoomState.scale === 1) {
      return;
    }
    if (!e.touches.length) {
      return;
    }
    const t = e.touches[0];
    touchStart.current = { x: t.clientX - zoomState.translateX, y: t.clientY - zoomState.translateY };
  };
  const handleTouchMoveNative = (e: TouchEvent) => {
    if (!touchStart.current) {
      return;
    }

    e.preventDefault();
    const t = e.touches[0];
    setZoomState((prev) => ({
      ...prev,
      translateX: t.clientX - touchStart.current!.x,
      translateY: t.clientY - touchStart.current!.y
    }));
  };
  const handleTouchEndNative = () => {
    touchStart.current = null;
  };

  useEffect(() => {
    const el = viewportRef.current;
    if (!el) {
      return;
    }

    el.addEventListener("wheel", handleWheelNative, { passive: false });
    el.addEventListener("touchstart", handleTouchStartNative, { passive: false });
    el.addEventListener("touchmove", handleTouchMoveNative, { passive: false });
    el.addEventListener("touchend", handleTouchEndNative, { passive: true });

    return () => {
      el.removeEventListener("wheel", handleWheelNative as EventListener);
      el.removeEventListener("touchstart", handleTouchStartNative as EventListener);
      el.removeEventListener("touchmove", handleTouchMoveNative as EventListener);
      el.removeEventListener("touchend", handleTouchEndNative as EventListener);
    };
  }, [image, zoomState.scale]);

  const handleViewportKeyDown: React.KeyboardEventHandler<HTMLDivElement> = (e) => {
    const step = 20;
    switch (e.key) {
      case "ArrowUp":
        e.preventDefault();
        setZoomState((p) => ({ ...p, translateY: p.translateY + step }));
        break;
      case "ArrowDown":
        e.preventDefault();
        setZoomState((p) => ({ ...p, translateY: p.translateY - step }));
        break;
      case "ArrowLeft":
        e.preventDefault();
        setZoomState((p) => ({ ...p, translateX: p.translateX + step }));
        break;
      case "ArrowRight":
        e.preventDefault();
        setZoomState((p) => ({ ...p, translateX: p.translateX - step }));
        break;
      case "Home":
        e.preventDefault();
        resetZoom();
        break;
      case "+":
      case "=":
        e.preventDefault();
        zoomIn();
        break;
      case "-":
      case "_":
        e.preventDefault();
        zoomOut();
        break;
      default:
        break;
    }
  };

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
          className={`flex flex-col gap-4 p-4 md:flex-row md:items-center md:justify-between md:p-6 ${
            theme === "dark"
              ? "bg-gradient-to-r from-gray-800 to-gray-800"
              : "bg-gradient-to-r from-purple-50 to-pink-50"
          } border-b ${themeClasses.border}`}
        >
          <div className="flex flex-col gap-2">
            <h3 id={headerId} className={`text-lg font-bold md:text-2xl ${themeClasses.text}`}>
              {image.label}
            </h3>
            <p id={descId} className="sr-only">
              Zoomable image viewer. Use mouse wheel to zoom, drag or touch to pan, arrow keys to pan, and Escape to
              close.
            </p>
            {container && (
              <span
                className={`w-fit rounded-full px-3 py-1 ${
                  theme === "dark" ? "bg-purple-900/30 text-purple-300" : "bg-purple-100 text-purple-700"
                } text-xs font-medium md:text-sm`}
              >
                {container.name}
              </span>
            )}
          </div>
          <div className="flex flex-wrap items-center gap-2">
            {onNavigate && (
              <>
                <button
                  type="button"
                  className={`rounded-lg px-3 py-2 ${themeClasses.bgTertiary} hover:${themeClasses.bgTertiary} ${themeClasses.text} transition-colors`}
                  onClick={() => onNavigate(-1)}
                  title="Previous (←)"
                >
                  ⬅️
                </button>
                <button
                  type="button"
                  className={`rounded-lg px-3 py-2 ${themeClasses.bgTertiary} hover:${themeClasses.bgTertiary} ${themeClasses.text} transition-colors`}
                  onClick={() => onNavigate(1)}
                  title="Next (→)"
                >
                  ➡️
                </button>
              </>
            )}
            <button
              type="button"
              className={`rounded-lg px-3 py-2 ${themeClasses.bgTertiary} hover:${themeClasses.bgTertiary} ${themeClasses.text} transition-colors`}
              onClick={zoomOut}
              title="Zoom Out (-)"
            >
              ➖
            </button>
            <span className={`px-3 py-2 font-mono text-sm ${themeClasses.textSecondary}`}>
              {Math.round(zoomState.scale * 100)}%
            </span>
            <button
              type="button"
              className={`rounded-lg px-3 py-2 ${themeClasses.bgTertiary} hover:${themeClasses.bgTertiary} ${themeClasses.text} transition-colors`}
              onClick={zoomIn}
              title="Zoom In (+)"
            >
              ➕
            </button>
            <button
              type="button"
              className="rounded-lg bg-gray-200 px-3 py-2 text-gray-800 transition-colors hover:bg-gray-300 dark:bg-gray-700 dark:text-white dark:hover:bg-gray-600"
              onClick={fitToScreen}
              title="Fit to Screen (F)"
            >
              📐
            </button>
            <button
              type="button"
              className="rounded-lg bg-gray-200 px-3 py-2 text-gray-800 transition-colors hover:bg-gray-300 dark:bg-gray-700 dark:text-white dark:hover:bg-gray-600"
              onClick={actualSize}
              title="Actual Size (0)"
            >
              1:1
            </button>
            <button
              type="button"
              className="rounded-lg bg-gray-200 px-3 py-2 text-gray-800 transition-colors hover:bg-gray-300 dark:bg-gray-700 dark:text-white dark:hover:bg-gray-600"
              onClick={resetZoom}
              title="Reset"
            >
              🔄
            </button>
            <button
              type="button"
              className="rounded-lg bg-gradient-to-r from-green-500 to-emerald-500 px-3 py-2 text-white transition-colors hover:from-green-600 hover:to-emerald-600"
              onClick={() => onDownload(image)}
              title="Download (D)"
            >
              💾
            </button>
            <button
              type="button"
              className="rounded-lg bg-red-500 px-3 py-2 font-bold text-white transition-colors hover:bg-red-600"
              onClick={onClose}
              title="Close (ESC)"
              aria-label="Close image modal"
            >
              ✕
            </button>
          </div>
        </div>

        <div
          ref={viewportRef}
          role="button"
          aria-label="Zoomable image viewer"
          tabIndex={0}
          className="flex flex-1 touch-none items-center justify-center overflow-hidden bg-gray-50 outline-none dark:bg-gray-950"
          onMouseDown={handleMouseDown}
          onMouseMove={handleMouseMove}
          onMouseUp={handleMouseUp}
          onMouseLeave={handleMouseUp}
          onKeyDown={handleViewportKeyDown}
          style={{ cursor: isDragging ? "grabbing" : zoomState.scale > 1 ? "grab" : "default" }}
        >
          <img
            src={`data:image/png;base64,${image.data}`}
            alt={image.label}
            className="max-h-full max-w-full object-contain"
            style={{
              transform: `scale(${zoomState.scale}) translate(${zoomState.translateX / zoomState.scale}px, ${zoomState.translateY / zoomState.scale}px)`,
              transition: isDragging ? "none" : "transform 0.1s ease-out"
            }}
            draggable={false}
          />
        </div>

        <div className="flex flex-col gap-3 border-t border-gray-200 bg-gray-100 p-4 md:flex-row md:items-center md:justify-between dark:border-gray-700 dark:bg-gray-800">
          <div className="flex flex-wrap gap-4 text-xs text-gray-600 md:text-sm dark:text-gray-400">
            <p>
              Size: {image.width} × {image.height}px
            </p>
            {image.metrics?.executionTimeMs && <p>Execution: {image.metrics.executionTimeMs.toFixed(2)}ms</p>}
            {image.metrics?.memoryUsageMB && <p>Memory: {image.metrics.memoryUsageMB.toFixed(2)}MB</p>}
          </div>
          <p className="text-xs text-gray-500 dark:text-gray-500">
            💡 Wheel: Zoom | Drag/Touch: Pan | Arrows: Pan | F: Fit | D: Download | ESC: Close
          </p>
        </div>
      </div>
    </div>
  );
};
