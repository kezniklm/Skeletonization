"use client";

import Image from "next/image";
import { Check } from "lucide-react";

import type { SelectImage } from "@/database/zod/image";
import { Badge } from "@/components/ui/badge";
import { Checkbox } from "@/components/ui/checkbox";
import { Tooltip, TooltipContent, TooltipTrigger } from "@/components/ui/tooltip";
import { cn } from "@/lib/utils";
import { type FilterType } from "@/components/filters";
import { SkeletonizationImageGalleryEmptyState } from "@/app/skeletonization/skeletonization-image-gallery-empty-state";

type SkeletonizationImageGalleryProps = {
  images: SelectImage[];
  allFilteredImages: SelectImage[];
  selectedImageIds: string[];
  onSelectionChange: (imageIds: string[]) => void;
  getShouldPreprocessImage: (imageId: string) => boolean;
  onTogglePreprocessImage: (imageId: string) => void;
  selectedFilter: FilterType;
};

export const SkeletonizationImageGallery = ({
  images,
  allFilteredImages,
  selectedImageIds,
  onSelectionChange,
  getShouldPreprocessImage,
  onTogglePreprocessImage,
  selectedFilter
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
    return <SkeletonizationImageGalleryEmptyState selectedFilter={selectedFilter} />;
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
          const shouldPreprocess = getShouldPreprocessImage(image.id);
          return (
            <div
              key={image.id}
              role="button"
              tabIndex={0}
              aria-pressed={isSelected}
              onClick={() => toggleImage(image.id)}
              onKeyDown={(e) => {
                if (e.key === "Enter" || e.key === " ") {
                  e.preventDefault();
                  toggleImage(image.id);
                }
              }}
              className={cn(
                "group relative aspect-square h-48 w-48 shrink-0 cursor-pointer overflow-hidden rounded-lg border-2 transition-all focus-visible:ring-2 focus-visible:ring-cyan-500 focus-visible:ring-offset-2 focus-visible:outline-none lg:h-12 lg:w-12 xl:h-40 xl:w-40 2xl:h-48 2xl:w-48 dark:focus-visible:ring-cyan-400 dark:focus-visible:ring-offset-gray-950",
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

                {isSelected && (
                  <div className="mt-1 flex items-center gap-2">
                    <Checkbox
                      checked={shouldPreprocess}
                      onClick={(e) => e.stopPropagation()}
                      onKeyDown={(e) => e.stopPropagation()}
                      onPointerDown={(e) => e.stopPropagation()}
                      onCheckedChange={() => onTogglePreprocessImage(image.id)}
                    />

                    <Tooltip>
                      <TooltipTrigger asChild>
                        <button
                          type="button"
                          className="text-[10px] text-gray-200"
                          onClick={(e) => {
                            e.stopPropagation();
                            onTogglePreprocessImage(image.id);
                          }}
                          onPointerDown={(e) => e.stopPropagation()}
                        >
                          Preprocess
                        </button>
                      </TooltipTrigger>
                      <TooltipContent sideOffset={6}>
                        Skeletonization algorithms expect binary images containing 0 and 1.
                      </TooltipContent>
                    </Tooltip>
                  </div>
                )}
              </div>
            </div>
          );
        })}
      </div>
    </div>
  );
};
