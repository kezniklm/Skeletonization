"use client";

import Image from "next/image";
import { Check } from "lucide-react";

import type { SelectImage } from "@/database/zod/image";
import { Badge } from "@/components/ui/badge";
import { cn } from "@/lib/utils";

type SkeletonizationImageGalleryProps = {
  images: SelectImage[];
  allFilteredImages: SelectImage[];
  selectedImageIds: string[];
  onSelectionChange: (imageIds: string[]) => void;
};

export const SkeletonizationImageGallery = ({
  images,
  allFilteredImages,
  selectedImageIds,
  onSelectionChange
}: SkeletonizationImageGalleryProps) => {
  const toggleImage = (imageId: string) => {
    if (selectedImageIds.includes(imageId)) {
      onSelectionChange(selectedImageIds.filter((id) => id !== imageId));
    } else {
      onSelectionChange([...selectedImageIds, imageId]);
    }
  };

  const selectAll = () => {
    const imagesToSelect = allFilteredImages.slice(0, 1000);
    onSelectionChange(imagesToSelect.map((img) => img.id));
  };

  const deselectAll = () => {
    onSelectionChange([]);
  };

  if (images.length === 0) {
    return (
      <div className="flex flex-col items-center justify-center rounded-lg border-2 border-dashed border-gray-300 py-12 text-center dark:border-gray-700">
        <p className="text-gray-600 dark:text-gray-400">No images available</p>
        <p className="mt-1 text-sm text-gray-500 dark:text-gray-500">
          Upload and validate images to start skeletonization
        </p>
      </div>
    );
  }

  return (
    <div className="space-y-3">
      <div className="flex items-center justify-between">
        <div className="flex items-center gap-2">
          <p className="text-sm font-medium text-gray-700 dark:text-gray-300">
            {selectedImageIds.length} of {allFilteredImages.length} selected
          </p>
          {selectedImageIds.length > 0 && (
            <Badge variant="secondary" className="h-5 px-2 text-xs">
              {selectedImageIds.length}
            </Badge>
          )}
        </div>
        <div className="flex gap-2">
          <button
            onClick={selectAll}
            className="text-sm font-medium text-cyan-600 hover:text-cyan-700 dark:text-cyan-400 dark:hover:text-cyan-300"
            type="button"
          >
            Select All{allFilteredImages.length > 1000 ? " (1000)" : ""}
          </button>
          <span className="text-gray-300 dark:text-gray-700">|</span>
          <button
            onClick={deselectAll}
            className="text-sm font-medium text-gray-600 hover:text-gray-700 dark:text-gray-400 dark:hover:text-gray-300"
            type="button"
          >
            Deselect All
          </button>
        </div>
      </div>

      <div className="flex gap-3 overflow-x-auto pb-2">
        {images.map((image) => {
          const isSelected = selectedImageIds.includes(image.id);
          return (
            <button
              key={image.id}
              type="button"
              onClick={() => toggleImage(image.id)}
              className={cn(
                "group relative aspect-square h-48 w-48 shrink-0 overflow-hidden rounded-lg border-2 transition-all lg:h-12 lg:w-12 xl:h-40 xl:w-40 2xl:h-48 2xl:w-48",
                isSelected
                  ? "border-cyan-500 dark:border-cyan-400"
                  : "border-gray-200 hover:border-gray-300 dark:border-gray-700 dark:hover:border-gray-600"
              )}
            >
              <Image
                src={image.url}
                alt={image.originalFilename}
                width={image.width}
                height={image.height}
                className="h-full w-full object-cover transition-transform group-hover:scale-105"
              />
              <div
                className={cn(
                  "absolute inset-0 bg-black/40 transition-opacity",
                  isSelected ? "opacity-100" : "opacity-0 group-hover:opacity-100"
                )}
              />
              {isSelected && (
                <div className="absolute top-2 right-2 flex h-6 w-6 items-center justify-center rounded-full bg-cyan-500 text-white shadow-lg dark:bg-cyan-400">
                  <Check className="h-4 w-4" />
                </div>
              )}
              <div className="absolute inset-x-0 bottom-0 bg-linear-to-t from-black/60 to-transparent p-2">
                <p className="truncate text-xs font-medium text-white">{image.originalFilename}</p>
                <p className="text-[10px] text-gray-300">
                  {image.width} × {image.height}
                </p>
              </div>
            </button>
          );
        })}
      </div>
    </div>
  );
};
