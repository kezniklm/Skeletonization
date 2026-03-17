/**
 * @file ImageGallery.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Renders the image grid for one benchmark container.
 * @description Maps container images into actionable image cards used by the main gallery view.
 * @version 1.0
 * @date 2026-03-16
 */

import type { ImageData, ImageContainer } from "../../../types";

import { ImageCard } from "./ImageCard";

type ImageGalleryProps = {
  container: ImageContainer;
  onImageClick: (image: ImageData, container: ImageContainer) => void;
  onDownload: (image: ImageData) => void;
  selectedComparisonIds: string[];
  onToggleComparisonSelection: (image: ImageData) => void;
};

/**
 * @brief Displays all images for a selected benchmark container.
 * @param container Container with source and processed images.
 * @param onImageClick Callback for opening image preview.
 * @param onDownload Callback for image download.
 * @param selectedComparisonIds IDs selected for multi-image comparison.
 * @param onToggleComparisonSelection Callback for selection updates.
 * @returns Image grid JSX.
 */
/** @brief Renders a grid of image cards for one container. */
export const ImageGallery = ({
  container,
  onImageClick,
  onDownload,
  selectedComparisonIds,
  onToggleComparisonSelection
}: ImageGalleryProps) => (
  <div className="grid grid-cols-1 gap-6 md:grid-cols-2 lg:grid-cols-3 xl:grid-cols-4">
    {container.images.map((image, index) => (
      <ImageCard
        key={image.id}
        image={image}
        container={container}
        index={index}
        onImageClick={onImageClick}
        onDownload={onDownload}
        isSelectedForComparison={selectedComparisonIds.includes(image.id)}
        onToggleComparisonSelection={onToggleComparisonSelection}
      />
    ))}
  </div>
);
