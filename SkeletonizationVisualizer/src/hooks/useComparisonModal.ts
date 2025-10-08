import { useState } from "react";

import { APP_CONFIG } from "../constants";
import type { ComparisonMode, ImageData } from "../types";

export const useComparisonModal = () => {
  const [isOpen, setIsOpen] = useState(false);
  const [comparisonMode, setComparisonMode] = useState<ComparisonMode>("side-by-side");
  const [comparisonImages, setComparisonImages] = useState<[ImageData | null, ImageData | null]>([null, null]);
  const [sliderPosition, setSliderPosition] = useState<number>(APP_CONFIG.DEFAULT_SLIDER_POSITION);

  const openComparison = (original: ImageData, processed: ImageData) => {
    setComparisonImages([original, processed]);
    setIsOpen(true);
  };

  const closeComparison = () => {
    setComparisonImages([null, null]);
    setIsOpen(false);
  };

  return {
    isOpen,
    comparisonMode,
    comparisonImages,
    sliderPosition,
    setComparisonMode,
    setSliderPosition,
    openComparison,
    closeComparison
  };
};
