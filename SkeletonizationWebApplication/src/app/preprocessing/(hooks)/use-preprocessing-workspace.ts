"use client";

import { type CV } from "mirada/dist/src/types/opencv";
import { useRouter } from "next/navigation";
import { useEffect, useRef, useState, useTransition } from "react";

import type { FileOutputFormat, SelectImage } from "@/database/zod";
import { savePreprocessedImageAction } from "@/server-actions/images";

import { initialFilters, initialTransforms, type DrawingTool, type FilterState, type TransformState } from "../types";
import { drawCanvasToCanvas } from "../utils/canvas";
import { getExtension, getMimeType } from "../utils/format-mapper";

import { useCustomCodeExecutor } from "./use-custom-code-executor";
import { useDrawing } from "./use-drawing";
import { useFeedbackDialog } from "./use-feedback-dialog";
import { useProcessingHistory } from "./use-image-history";
import { usePreprocessingCanvas } from "./use-preprocessing-canvas";
import { useProcessingPipeline } from "./use-preprocessing-pipeline";
import { useSaveConfirmationDialog } from "./use-save-confirmation-dialog";

type UsePreprocessingWorkspaceArgs = {
  selectedImage: SelectImage;
  cv: CV;
  originalImage: HTMLImageElement;
  defaultOutputFormat: FileOutputFormat;
};

export const usePreprocessingWorkspace = ({
  selectedImage,
  cv,
  originalImage,
  defaultOutputFormat
}: UsePreprocessingWorkspaceArgs) => {
  const router = useRouter();
  const [, startTransition] = useTransition();

  const [filters, setFilters] = useState<FilterState>(initialFilters);
  const [transforms, setTransforms] = useState<TransformState>(initialTransforms);

  const [activeTool, setActiveTool] = useState<DrawingTool>("none");
  const [drawColor, setDrawColor] = useState("#000000");
  const [brushSize, setBrushSize] = useState(3);

  const [processing, setProcessing] = useState(false);
  const [saving, setSaving] = useState(false);
  const [showComparison, setShowComparison] = useState(false);

  const [customCode, setCustomCode] = useState("");
  const [codeError, setCodeError] = useState<string | null>(null);

  const skipNextProcessingRef = useRef(false);

  const { originalImageRef, canvasRef, comparisonCanvasRef, baseCanvasRef } = usePreprocessingCanvas(originalImage);

  const commitAsProcessingBase = () => {
    const canvas = canvasRef.current;
    if (!canvas) return;

    baseCanvasRef.current ??= document.createElement("canvas");

    drawCanvasToCanvas(baseCanvasRef.current, canvas);
  };

  const {
    historyIndex,
    historyLength,
    addToHistorySnapshot,
    initializeHistory,
    undo,
    redo,
    resetAll: resetHistory
  } = useProcessingHistory(
    setFilters,
    setTransforms,
    canvasRef,
    baseCanvasRef,
    originalImageRef,
    skipNextProcessingRef
  );

  const feedbackDialog = useFeedbackDialog();
  const saveConfirmationDialog = useSaveConfirmationDialog();

  useProcessingPipeline({
    cv,
    filters,
    transforms,
    originalImageRef,
    baseCanvasRef,
    canvasRef,
    comparisonCanvasRef,
    showComparison,
    setProcessing,
    skipNextProcessingRef
  });

  useEffect(() => {
    const timer = setTimeout(() => {
      initializeHistory();
    }, 100);
    return () => clearTimeout(timer);
  }, [initializeHistory]);

  const { executeCustomCode } = useCustomCodeExecutor({
    cv,
    canvasRef,
    setProcessing,
    setCodeError,
    commitAsProcessingBase,
    addToHistory: (desc) => addToHistorySnapshot(desc, filters, transforms)
  });

  const drawingHandlers = useDrawing({
    canvasRef,
    cv,
    activeTool,
    drawColor,
    brushSize,
    commitAsProcessingBase,
    addToHistory: (desc) => addToHistorySnapshot(desc, filters, transforms),
    onFinishCrop: () => setActiveTool("none")
  });

  const addToHistory = (description: string) => {
    requestAnimationFrame(() => {
      addToHistorySnapshot(description, filters, transforms);
    });
  };

  const resetFilters = () => {
    setFilters(initialFilters);
    addToHistorySnapshot("Reset all filters", initialFilters, transforms);
  };

  const resetTransforms = () => {
    setTransforms(initialTransforms);
    addToHistorySnapshot("Reset all transforms", filters, initialTransforms);
  };

  const resetAll = () => {
    resetHistory();
  };

  const applyPreset = (preset: string) => {
    setFilters((prev) => {
      switch (preset) {
        case "enhance": {
          const next = { ...prev, sharpen: 2, brightness: 10, contrast: 1.2 };
          addToHistorySnapshot("Applied Enhancement preset", next, transforms);
          return next;
        }
        case "bw": {
          const next = { ...prev, grayscale: true, contrast: 1.3, brightness: 5 };
          addToHistorySnapshot("Applied Black & White preset", next, transforms);
          return next;
        }
        case "smooth": {
          const next = { ...prev, blur: 2, denoise: 5 };
          addToHistorySnapshot("Applied Smooth preset", next, transforms);
          return next;
        }
        case "vivid": {
          const next = { ...prev, saturation: 1.5, contrast: 1.2 };
          addToHistorySnapshot("Applied Vivid preset", next, transforms);
          return next;
        }
        default:
          return prev;
      }
    });
  };

  const downloadImage = () => {
    const canvas = canvasRef.current;
    if (!canvas) return;

    const mimeType = getMimeType(defaultOutputFormat);
    const extension = getExtension(defaultOutputFormat);
    const link = document.createElement("a");
    link.download = `${selectedImage.originalFilename.replace(/\.[^/.]+$/, "")}_processed.${extension}`;
    link.href = canvas.toDataURL(mimeType);
    link.click();
  };

  const saveImage = async () => {
    const canvas = canvasRef.current;
    if (!canvas) return;

    setSaving(true);
    try {
      const mimeType = getMimeType(defaultOutputFormat);
      const dataUrl = canvas.toDataURL(mimeType);

      await savePreprocessedImageAction(selectedImage.id, dataUrl, defaultOutputFormat);
      saveConfirmationDialog.openDialog();
    } catch (err) {
      console.error("Save error:", err);
      const msg = err instanceof Error ? err.message : "Failed to save image";
      feedbackDialog.showFeedback(msg, "error");
    } finally {
      setSaving(false);
    }
  };

  const selectImage = (imageId?: string) => {
    startTransition(() => {
      if (imageId) {
        router.push(`/preprocessing?imageId=${imageId}`);
      } else {
        router.push("/preprocessing");
      }
    });
  };

  const handleExecuteCode = () => {
    executeCustomCode(customCode);
  };

  const previewProps = {
    canvasRef,
    comparisonCanvasRef,
    showComparison,
    activeTool,
    onChangeImage: () => selectImage(),
    onMouseDown: drawingHandlers.onMouseDown,
    onMouseMove: drawingHandlers.onMouseMove,
    onMouseUp: drawingHandlers.onMouseUp,
    onMouseLeave: drawingHandlers.onMouseLeave
  };

  const controlsProps = {
    filters,
    setFilters,
    transforms,
    setTransforms,
    onResetFilters: resetFilters,
    onResetTransforms: resetTransforms,
    onResetAll: resetAll,
    onApplyPreset: applyPreset,
    historyIndex,
    historyLength,
    onUndo: undo,
    onRedo: redo,
    showComparison,
    onToggleComparison: () => setShowComparison((prev) => !prev),
    onDownload: downloadImage,
    onSave: saveImage,
    saving,
    activeTool,
    setActiveTool,
    drawColor,
    setDrawColor,
    brushSize,
    setBrushSize,
    customCode,
    setCustomCode,
    codeError,
    onExecuteCode: handleExecuteCode,
    processing,
    onAddHistory: addToHistory
  };

  return {
    previewProps,
    controlsProps,
    feedbackDialogProps: feedbackDialog,
    saveConfirmationDialogProps: saveConfirmationDialog
  };
};
