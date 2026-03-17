/**
 * @file use-custom-code-executor.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Provides custom OpenCV code execution for preprocessing canvas state.
 * @description Exposes a helper hook that evaluates user-provided scripts against source and destination Mats and commits results to history.
 */

"use client";

import { type CV, type Mat } from "mirada/dist/src/types/opencv";
import type { Dispatch, RefObject, SetStateAction } from "react";

import { get2DContextOrThrow, getCanvasOrThrow } from "../utils/canvas";

type Args = {
  cv: CV;
  canvasRef: RefObject<HTMLCanvasElement>;
  setProcessing: Dispatch<SetStateAction<boolean>>;
  setCodeError: Dispatch<SetStateAction<string | null>>;
  commitAsProcessingBase: () => void;
  addToHistory: (description: string) => void;
};

/**
 * @brief Creates a custom code execution handler for preprocessing.
 * @description Wraps canvas extraction, script execution, error handling, and resource cleanup for user-authored OpenCV code.
 * @param cv OpenCV runtime instance.
 * @param canvasRef Reference to the target preprocessing canvas.
 * @param setProcessing Setter controlling processing busy state.
 * @param setCodeError Setter for user-code error messages.
 * @param commitAsProcessingBase Callback to persist current output as pipeline base.
 * @param addToHistory Callback to append history entries.
 * @returns An object containing `executeCustomCode` to run user scripts.
 */
/** @brief Provides execution of user-defined OpenCV code on the canvas. */
export const useCustomCodeExecutor = ({
  cv,
  canvasRef,
  setProcessing,
  setCodeError,
  commitAsProcessingBase,
  addToHistory
}: Args) => {
  const deleteMatSafe = (mat: Mat | null) => {
    try {
      if (mat && !mat.isDeleted()) {
        mat.delete();
      }
    } catch {
      /* empty */
    }
  };

  const createSourceAndDestinationMats = (canvas: HTMLCanvasElement, ctx: CanvasRenderingContext2D) => {
    const imageData = ctx.getImageData(0, 0, canvas.width, canvas.height);

    const src = cv.matFromImageData(imageData);
    const dst = new cv.Mat();
    src.copyTo(dst);

    return { src, dst };
  };

  const runUserCode = (customCode: string, src: Mat, dst: Mat) => {
    const userFunction = new Function("cv", "src", "dst", customCode) as (cv: CV, src: Mat, dst: Mat) => void;

    userFunction(cv, src, dst);
  };

  const renderResultToCanvas = (canvas: HTMLCanvasElement, dst: Mat | null) => {
    if (dst && !dst.isDeleted()) {
      cv.imshow(canvas, dst);
      commitAsProcessingBase();
      addToHistory("Executed custom code");
    }
  };

  const handleExecutionError = (err: unknown) => {
    const errorMessage = err instanceof Error ? err.message : String(err);
    setCodeError(`Code execution error: ${errorMessage}`);
    console.error("Code execution error:", err);
  };

  const cleanup = (src: Mat | null, dst: Mat | null) => {
    deleteMatSafe(src);
    deleteMatSafe(dst);
    setProcessing(false);
  };

  const executeCustomCode = (customCode: string) => {
    if (!customCode) return;

    setProcessing(true);
    setCodeError(null);

    let src: Mat | null = null;
    let dst: Mat | null = null;

    try {
      const canvas = getCanvasOrThrow(canvasRef);
      const ctx = get2DContextOrThrow(canvas);
      ({ src, dst } = createSourceAndDestinationMats(canvas, ctx));
      runUserCode(customCode, src, dst);
      renderResultToCanvas(canvas, dst);
    } catch (err) {
      handleExecutionError(err);
    } finally {
      cleanup(src, dst);
    }
  };

  return { executeCustomCode };
};
