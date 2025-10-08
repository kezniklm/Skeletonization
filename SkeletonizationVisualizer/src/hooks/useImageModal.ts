import { useState } from "react";

import type { ImageContainer, ImageData } from "../types";

export const useImageModal = () => {
  const [isOpen, setIsOpen] = useState(false);
  const [selectedImage, setSelectedImage] = useState<ImageData | null>(null);
  const [selectedContainer, setSelectedContainer] = useState<ImageContainer | null>(null);

  const openImageModal = (image: ImageData, container: ImageContainer) => {
    setSelectedImage(image);
    setSelectedContainer(container);
    setIsOpen(true);
  };

  const closeImageModal = () => {
    setSelectedImage(null);
    setSelectedContainer(null);
    setIsOpen(false);
  };

  const navigateToImage = (image: ImageData, container: ImageContainer) => {
    setSelectedImage(image);
    setSelectedContainer(container);
  };

  return {
    isOpen,
    selectedImage,
    selectedContainer,
    openImageModal,
    closeImageModal,
    navigateToImage
  };
};
