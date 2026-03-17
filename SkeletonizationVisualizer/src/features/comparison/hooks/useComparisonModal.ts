/**
 * @file useComparisonModal.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Implements state container for comparison modal workflow.
 * @description Stores selected original and processed images, mode selection, and slider state for comparison UI.
 * @version 1.0
 * @date 2026-03-16
 */

import { useState } from "react";

import { APP_CONFIG } from "../../../constants";
import type { ComparisonMode, ImageData } from "../../../types";

/**
 * @brief Manages comparison modal state and handlers.
 * @returns Comparison modal state and control callbacks.
 * @example
 * const { isOpen, openComparison, closeComparison } = useComparisonModal();
 */
export const useComparisonModal = () => {
  const [isOpen, setIsOpen] = useState(false);
  const [comparisonMode, setComparisonMode] = useState<ComparisonMode>("side-by-side");
  const [comparisonOriginal, setComparisonOriginal] = useState<ImageData | null>(null);
  const [comparisonProcessed, setComparisonProcessed] = useState<ImageData[]>([]);
  const [sliderPosition, setSliderPosition] = useState<number>(APP_CONFIG.DEFAULT_SLIDER_POSITION);

  /**
   * @brief Opens comparison with original and selected processed images.
   * @param original Original source image.
   * @param processed Processed images selected for comparison.
   * @returns No return value.
   */
  const openComparison = (original: ImageData, processed: ImageData[]) => {
    if (!processed.length) {
      return;
    }
    setComparisonOriginal(original);
    setComparisonProcessed(processed);
    setIsOpen(true);
  };

  /**
   * @brief Resets and closes comparison modal state.
   * @returns No return value.
   */
  const closeComparison = () => {
    setComparisonOriginal(null);
    setComparisonProcessed([]);
    setIsOpen(false);
  };

  return {
    isOpen,
    comparisonMode,
    comparisonOriginal,
    comparisonProcessed,
    sliderPosition,
    setComparisonMode,
    setSliderPosition,
    openComparison,
    closeComparison
  };
};
