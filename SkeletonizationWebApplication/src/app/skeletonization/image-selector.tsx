/**
 * @file image-selector.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Renders selectable image tiles for skeletonization setup.
 * @description Provides image selection toggles, select-all behavior, and empty-state feedback.
 */

"use client";

import { Check, Image as ImageIcon } from "lucide-react";
import Image from "next/image";

import { Card, CardContent } from "@/components/ui/card";
import { type SelectImage } from "@/database/zod/image";

type ImageSelectorProps = {
  images: SelectImage[];
  selectedImageIds: string[];
  onSelectionChange: (imageIds: string[]) => void;
};

/**
 * @brief Displays a selectable gallery of candidate images.
 * @description Allows users to select individual or all images and communicates selection updates to the parent workspace.
 * @param images Available images for selection.
 * @param selectedImageIds Currently selected image IDs.
 * @param onSelectionChange Callback receiving updated selected image IDs.
 * @returns A selection grid or empty-state card.
 */
export const ImageSelector = ({ images, selectedImageIds, onSelectionChange }: ImageSelectorProps) => {
  const toggleImage = (imageId: string) => {
    if (selectedImageIds.includes(imageId)) {
      onSelectionChange(selectedImageIds.filter((id) => id !== imageId));
    } else {
      onSelectionChange([...selectedImageIds, imageId]);
    }
  };

  const toggleAll = () => {
    if (selectedImageIds.length === images.length) {
      onSelectionChange([]);
    } else {
      onSelectionChange(images.map((img) => img.id));
    }
  };

  return (
    <div className="space-y-4">
      <div className="flex items-center justify-between">
        <h3 className="text-lg font-semibold text-gray-900 dark:text-white">
          Select Images ({selectedImageIds.length} of {images.length})
        </h3>
        <button
          onClick={toggleAll}
          className="text-sm font-medium text-cyan-600 hover:text-cyan-700 dark:text-cyan-400 dark:hover:text-cyan-300"
        >
          {selectedImageIds.length === images.length ? "Deselect All" : "Select All"}
        </button>
      </div>

      {images.length === 0 ? (
        <Card>
          <CardContent className="flex flex-col items-center justify-center py-12">
            <div className="rounded-full bg-gray-100 p-4 dark:bg-gray-800">
              <ImageIcon className="h-8 w-8 text-gray-400" />
            </div>
            <p className="mt-4 text-sm text-gray-600 dark:text-gray-400">No images available</p>
            <p className="mt-1 text-xs text-gray-500 dark:text-gray-500">Upload images to get started</p>
          </CardContent>
        </Card>
      ) : (
        <div className="grid grid-cols-2 gap-4 sm:grid-cols-3 lg:grid-cols-4">
          {images.map((image) => {
            const isSelected = selectedImageIds.includes(image.id);
            return (
              <button
                key={image.id}
                type="button"
                onClick={() => toggleImage(image.id)}
                aria-pressed={isSelected}
                className={`group relative overflow-hidden rounded-lg border-2 transition-all ${
                  isSelected
                    ? "border-cyan-500 ring-2 ring-cyan-500 ring-offset-2 dark:border-cyan-400 dark:ring-cyan-400"
                    : "border-gray-200 hover:border-cyan-300 dark:border-gray-700 dark:hover:border-cyan-600"
                }`}
              >
                <div className="aspect-square overflow-hidden bg-gray-100 dark:bg-gray-800">
                  <Image
                    src={image.url}
                    alt={image.originalFilename}
                    className="h-full w-full object-cover transition-transform group-hover:scale-105"
                  />
                </div>

                <div className="absolute top-2 left-2">
                  <div
                    className={`flex h-6 w-6 items-center justify-center rounded border-2 ${
                      isSelected
                        ? "border-cyan-500 bg-cyan-500 dark:border-cyan-400 dark:bg-cyan-400"
                        : "border-white bg-white/80 dark:border-gray-600 dark:bg-gray-800/80"
                    }`}
                  >
                    {isSelected && <Check className="h-4 w-4 text-white dark:text-gray-900" />}
                  </div>
                </div>

                <div className="absolute inset-x-0 bottom-0 bg-linear-to-t from-black/60 to-transparent p-2">
                  <p className="truncate text-xs font-medium text-white">{image.originalFilename}</p>
                </div>
              </button>
            );
          })}
        </div>
      )}
    </div>
  );
};
