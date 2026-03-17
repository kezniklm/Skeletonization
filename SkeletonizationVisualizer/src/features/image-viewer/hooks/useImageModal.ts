/**
 * @file useImageModal.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Implements image modal selection state hook.
 * @description Stores selected image and container and exposes handlers for opening, closing, and navigation updates.
 * @version 1.0
 * @date 2026-03-16
 */

import { useState } from "react";

import type { ImageContainer, ImageData } from "../../../types";

/**
 * @brief Manages image modal state and handlers.
 * @returns Modal state and selection callbacks.
 */
export const useImageModal = () => {
  const [isOpen, setIsOpen] = useState(false);
  const [selectedImage, setSelectedImage] = useState<ImageData | null>(null);
  const [selectedContainer, setSelectedContainer] = useState<ImageContainer | null>(null);

  /**
   * @brief Opens modal for selected image.
   * @param image Image to present.
   * @param container Parent benchmark container.
   * @returns No return value.
   */
  const openImageModal = (image: ImageData, container: ImageContainer) => {
    setSelectedImage(image);
    setSelectedContainer(container);
    setIsOpen(true);
  };

  /**
   * @brief Closes modal and clears current selection.
   * @returns No return value.
   */
  const closeImageModal = () => {
    setSelectedImage(null);
    setSelectedContainer(null);
    setIsOpen(false);
  };

  /**
   * @brief Updates selection during in-modal navigation.
   * @param image Next selected image.
   * @param container Parent benchmark container of next image.
   * @returns No return value.
   */
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
