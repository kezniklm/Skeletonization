import { useState } from "react";

import { APP_CONFIG } from "../constants";
import type { ComparisonMode, ImageData } from "../types";

export const useComparisonModal = () => {
  const [isOpen, setIsOpen] = useState(false);
  const [comparisonMode, setComparisonMode] = useState<ComparisonMode>("side-by-side");
  const [comparisonOriginal, setComparisonOriginal] = useState<ImageData | null>(null);
  const [comparisonProcessed, setComparisonProcessed] = useState<ImageData[]>([]);
  const [sliderPosition, setSliderPosition] = useState<number>(APP_CONFIG.DEFAULT_SLIDER_POSITION);

  const openComparison = (original: ImageData, processed: ImageData[]) => {
    if (!processed.length) {
      return;
    }
    setComparisonOriginal(original);
    setComparisonProcessed(processed);
    setIsOpen(true);
  };

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
