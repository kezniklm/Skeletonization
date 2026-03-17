/**
 * @file useImageNavigation.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Implements keyboard/button image navigation logic.
 * @description Resolves previous and next images across all benchmark containers and updates selected image state.
 * @version 1.0
 * @date 2026-03-16
 */

import { useCallback } from "react";

import type { BenchmarkData, ImageContainer, ImageData } from "../../../types";

type UseImageNavigationProps = {
  data: BenchmarkData | null;
  selectedImage: ImageData | null;
  onImageSelect: (image: ImageData, container: ImageContainer) => void;
};

/**
 * @brief Provides image navigation callback for modal viewer.
 * @param data Benchmark data source.
 * @param selectedImage Currently selected image.
 * @param onImageSelect Callback invoked with next selected image.
 * @returns Navigation callback object.
 */
export const useImageNavigation = ({ data, selectedImage, onImageSelect }: UseImageNavigationProps) => {
  const navigateImage = useCallback(
    (direction: number) => {
      if (!data || !selectedImage) {
        return;
      }

      const allImages = data.containers.flatMap((c) => c.images);
      const currentIndex = allImages.findIndex((img) => img.id === selectedImage.id);
      const newIndex = (currentIndex + direction + allImages.length) % allImages.length;
      const newImage = allImages[newIndex];

      const newContainer = data.containers.find((container) => container.images.some((img) => img.id === newImage.id));

      if (newImage && newContainer) {
        onImageSelect(newImage, newContainer);
      }
    },
    [data, selectedImage, onImageSelect]
  );

  return {
    navigateImage
  };
};
