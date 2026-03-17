/**
 * @file use-image-history.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Tracks preprocessing undo/redo history and snapshot restoration.
 * @description Maintains state snapshots for filters, transforms, and canvases, and provides keyboard-aware navigation controls.
 */

"use client";

import { type Dispatch, type RefObject, type SetStateAction, useEffect, useState } from "react";

import {
  type FilterState,
  type HistoryEntry,
  initialFilters,
  initialTransforms,
  type TransformState
} from "@/app/preprocessing/types";

import { drawDataUrlToCanvas, drawImageElementToCanvas, getCanvasDataUrl } from "../utils/canvas";

type ApplyHistoryEntryParams = {
  entry: HistoryEntry;
  index: number;
  setFilters: Dispatch<SetStateAction<FilterState>>;
  setTransforms: Dispatch<SetStateAction<TransformState>>;
  setHistoryIndex: Dispatch<SetStateAction<number>>;
  canvasRef: RefObject<HTMLCanvasElement | null>;
  baseCanvasRef: RefObject<HTMLCanvasElement | null>;
  skipNextProcessingRef?: RefObject<boolean>;
};

const applyHistoryEntry = ({
  entry,
  index,
  setFilters,
  setTransforms,
  setHistoryIndex,
  canvasRef,
  baseCanvasRef,
  skipNextProcessingRef
}: ApplyHistoryEntryParams) => {
  if (skipNextProcessingRef) {
    skipNextProcessingRef.current = true;
  }

  setFilters(entry.filters);
  setTransforms(entry.transforms);

  const canvas = canvasRef.current;
  if (canvas && entry.canvasDataUrl) {
    drawDataUrlToCanvas(canvas, entry.canvasDataUrl);
  }

  const baseCanvas = baseCanvasRef.current;
  if (baseCanvas && entry.baseCanvasDataUrl) {
    drawDataUrlToCanvas(baseCanvas, entry.baseCanvasDataUrl);
  }

  setHistoryIndex(index);
};

/**
 * @brief Manages preprocessing state history for undo/redo operations.
 * @description Captures canvas snapshots with filter and transform states, supports keyboard shortcuts, and restores prior entries.
 * @param setFilters Setter for filter state restoration.
 * @param setTransforms Setter for transform state restoration.
 * @param canvasRef Reference to processed output canvas.
 * @param baseCanvasRef Reference to base processing canvas.
 * @param originalImageRef Reference to original image element.
 * @param skipNextProcessingRef Optional flag to suppress one pipeline run during history restore.
 * @returns History state metadata and mutation handlers.
 */
/** @brief Manages preprocessing undo/redo history snapshots. */
export const useProcessingHistory = (
  setFilters: Dispatch<SetStateAction<FilterState>>,
  setTransforms: Dispatch<SetStateAction<TransformState>>,
  canvasRef: RefObject<HTMLCanvasElement | null>,
  baseCanvasRef: RefObject<HTMLCanvasElement | null>,
  originalImageRef: RefObject<HTMLImageElement | null>,
  skipNextProcessingRef?: RefObject<boolean>
) => {
  const [history, setHistory] = useState<HistoryEntry[]>([]);
  const [historyIndex, setHistoryIndex] = useState(-1);

  const addToHistorySnapshot = (description: string, filters: FilterState, transforms: TransformState) => {
    const canvasDataUrl = getCanvasDataUrl(canvasRef);
    const baseCanvasDataUrl = getCanvasDataUrl(baseCanvasRef);
    if (!canvasDataUrl || !baseCanvasDataUrl) return;

    const entry: HistoryEntry = {
      filters: { ...filters },
      transforms: { ...transforms },
      canvasDataUrl,
      baseCanvasDataUrl,
      timestamp: Date.now(),
      description
    };

    setHistory((prev) => {
      const base = prev.slice(0, historyIndex + 1);
      const next = [...base, entry];
      setHistoryIndex(next.length - 1);
      return next;
    });
  };

  const undo = () => {
    if (historyIndex <= 0) return;
    const prevEntry = history[historyIndex - 1];

    applyHistoryEntry({
      entry: prevEntry,
      index: historyIndex - 1,
      setFilters,
      setTransforms,
      setHistoryIndex,
      canvasRef,
      baseCanvasRef,
      skipNextProcessingRef
    });
  };

  const redo = () => {
    if (historyIndex >= history.length - 1) return;
    const nextEntry = history[historyIndex + 1];

    applyHistoryEntry({
      entry: nextEntry,
      index: historyIndex + 1,
      setFilters,
      setTransforms,
      setHistoryIndex,
      canvasRef,
      baseCanvasRef,
      skipNextProcessingRef
    });
  };

  const resetAll = () => {
    const canvas = canvasRef.current;
    const img = originalImageRef.current;

    if (canvas && img) {
      drawImageElementToCanvas(canvas, img);
    }

    if (baseCanvasRef.current && img) {
      drawImageElementToCanvas(baseCanvasRef.current, img);
    }

    setFilters(initialFilters);
    setTransforms(initialTransforms);
    setHistory([]);
    setHistoryIndex(-1);
  };

  const initializeHistory = () => {
    if (history.length !== 0) return;

    const canvasDataUrl = getCanvasDataUrl(canvasRef);
    const baseCanvasDataUrl = getCanvasDataUrl(baseCanvasRef);
    if (!canvasDataUrl || !baseCanvasDataUrl) return;

    const entry: HistoryEntry = {
      filters: { ...initialFilters },
      transforms: { ...initialTransforms },
      canvasDataUrl,
      baseCanvasDataUrl,
      timestamp: Date.now(),
      description: "Initial state"
    };

    setHistory([entry]);
    setHistoryIndex(0);
  };

  useEffect(() => {
    const handleKeyDown = (e: KeyboardEvent) => {
      if (!(e.ctrlKey || e.metaKey)) return;

      if (e.key === "z" && !e.shiftKey) {
        e.preventDefault();
        if (historyIndex <= 0) return;

        const prevEntry = history[historyIndex - 1];
        applyHistoryEntry({
          entry: prevEntry,
          index: historyIndex - 1,
          setFilters,
          setTransforms,
          setHistoryIndex,
          canvasRef,
          baseCanvasRef,
          skipNextProcessingRef
        });
      } else if (e.key === "y" || (e.key === "z" && e.shiftKey)) {
        e.preventDefault();
        if (historyIndex >= history.length - 1) return;

        const nextEntry = history[historyIndex + 1];
        applyHistoryEntry({
          entry: nextEntry,
          index: historyIndex + 1,
          setFilters,
          setTransforms,
          setHistoryIndex,
          canvasRef,
          baseCanvasRef,
          skipNextProcessingRef
        });
      }
    };

    window.addEventListener("keydown", handleKeyDown);
    return () => window.removeEventListener("keydown", handleKeyDown);
  }, [history, historyIndex, setFilters, setTransforms, canvasRef, baseCanvasRef, skipNextProcessingRef]);

  return {
    historyIndex,
    historyLength: history.length,
    addToHistorySnapshot,
    initializeHistory,
    undo,
    redo,
    resetAll
  };
};
