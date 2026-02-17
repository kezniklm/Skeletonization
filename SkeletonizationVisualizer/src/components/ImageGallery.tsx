import type { ImageData, ImageContainer } from "../types";

import { ImageCard } from "./ImageCard";

type ImageGalleryProps = {
  container: ImageContainer;
  onImageClick: (image: ImageData, container: ImageContainer) => void;
  onDownload: (image: ImageData) => void;
  selectedComparisonIds: string[];
  onToggleComparisonSelection: (image: ImageData) => void;
};

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
