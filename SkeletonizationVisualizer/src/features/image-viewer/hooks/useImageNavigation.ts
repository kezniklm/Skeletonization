import { useCallback } from "react";

import type { BenchmarkData, ImageContainer, ImageData } from "../../../types";

type UseImageNavigationProps = {
  data: BenchmarkData | null;
  selectedImage: ImageData | null;
  onImageSelect: (image: ImageData, container: ImageContainer) => void;
};

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
