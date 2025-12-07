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
  skipNextProcessingRef?: RefObject<boolean>;
};

const applyHistoryEntry = ({
  entry,
  index,
  setFilters,
  setTransforms,
  setHistoryIndex,
  canvasRef,
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

  setHistoryIndex(index);
};

export const useProcessingHistory = (
  setFilters: Dispatch<SetStateAction<FilterState>>,
  setTransforms: Dispatch<SetStateAction<TransformState>>,
  canvasRef: RefObject<HTMLCanvasElement | null>,
  originalImageRef: RefObject<HTMLImageElement | null>,
  skipNextProcessingRef?: RefObject<boolean>
) => {
  const [history, setHistory] = useState<HistoryEntry[]>([]);
  const [historyIndex, setHistoryIndex] = useState(-1);

  const addToHistorySnapshot = (description: string, filters: FilterState, transforms: TransformState) => {
    const canvasDataUrl = getCanvasDataUrl(canvasRef);
    if (!canvasDataUrl) return;

    const entry: HistoryEntry = {
      filters: { ...filters },
      transforms: { ...transforms },
      canvasDataUrl,
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
      skipNextProcessingRef
    });
  };

  const resetAll = () => {
    const canvas = canvasRef.current;
    const img = originalImageRef.current;

    if (canvas && img) {
      drawImageElementToCanvas(canvas, img);
    }

    setFilters(initialFilters);
    setTransforms(initialTransforms);
    setHistory([]);
    setHistoryIndex(-1);
  };

  const initializeHistory = () => {
    if (history.length !== 0) return;

    const canvasDataUrl = getCanvasDataUrl(canvasRef);
    if (!canvasDataUrl) return;

    const entry: HistoryEntry = {
      filters: { ...initialFilters },
      transforms: { ...initialTransforms },
      canvasDataUrl,
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
          skipNextProcessingRef
        });
      }
    };

    window.addEventListener("keydown", handleKeyDown);
    return () => window.removeEventListener("keydown", handleKeyDown);
  }, [history, historyIndex, setFilters, setTransforms, canvasRef, skipNextProcessingRef]);

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
