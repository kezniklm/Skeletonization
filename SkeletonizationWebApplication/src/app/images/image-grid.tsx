"use client";

import { ImageIcon } from "lucide-react";

import type { SelectImage } from "@/database/zod/image";
import { Card } from "@/components/ui/card";

import { ImageUpload } from "./image-upload";
import { ImageCard } from "./image-card";

type ImageGridProps = {
  images: SelectImage[];
  filteredImages: SelectImage[];
  searchQuery: string;
  hasAdvancedFilters: boolean;
  selectedFilter: string;
  onUploadComplete: (image: SelectImage) => void;
  onDelete: (imageId: string) => void;
  onRename: (imageId: string, newFilename: string) => void;
  onArchive: (imageId: string) => void;
  onUnarchive: (imageId: string) => void;
};

export const ImageGrid = ({
  images,
  filteredImages,
  searchQuery,
  hasAdvancedFilters,
  selectedFilter,
  onUploadComplete,
  onDelete,
  onRename,
  onArchive,
  onUnarchive
}: ImageGridProps) => {
  const hasAnyImages = images.length > 0;
  const hasAnyFilteredImages = filteredImages.length > 0;
  const showUploadCard = filteredImages.length > 0 || (!searchQuery && !hasAdvancedFilters && !hasAnyImages);
  const showEmptyState =
    filteredImages.length === 0 &&
    (searchQuery ||
      hasAdvancedFilters ||
      (selectedFilter !== "all" && hasAnyImages) ||
      (selectedFilter === "all" && images.length > 0 && images.every((img) => img.status === "archived")));

  return (
    <div className="mt-6 space-y-4 xl:mt-5 xl:space-y-3 2xl:mt-6 2xl:space-y-4">
      {hasAnyFilteredImages && (
        <div className="text-muted-foreground flex items-center gap-2 text-sm xl:text-xs 2xl:text-sm">
          <div className="bg-primary h-1 w-1 rounded-full" />
          Showing <span className="text-foreground font-semibold">{filteredImages.length}</span> of{" "}
          <span className="text-foreground font-semibold">{images.length}</span> images
        </div>
      )}

      <div className="grid grid-cols-1 gap-6 sm:grid-cols-2 xl:gap-5 2xl:gap-6">
        {showUploadCard && (
          <ImageUpload onUploadComplete={onUploadComplete} compact fullWidth={filteredImages.length === 0} />
        )}

        {filteredImages.map((image) => (
          <ImageCard
            key={image.id}
            image={image}
            onDelete={onDelete}
            onRename={onRename}
            onArchive={onArchive}
            onUnarchive={onUnarchive}
          />
        ))}

        {showEmptyState && (
          <Card className="col-span-full overflow-hidden border-2 border-dashed border-cyan-300 bg-linear-to-br from-cyan-50/50 to-blue-50/50 backdrop-blur-sm xl:rounded-lg 2xl:rounded-xl dark:border-cyan-700 dark:from-cyan-950/20 dark:to-blue-950/20">
            <div className="relative flex aspect-64/27 w-full flex-col items-center justify-center px-4">
              <div className="rounded-full bg-cyan-100 p-3 xl:p-2.5 2xl:p-3 dark:bg-cyan-900/30">
                <ImageIcon className="h-8 w-8 text-cyan-600 xl:h-7 xl:w-7 2xl:h-8 2xl:w-8 dark:text-cyan-400" />
              </div>
              <h3 className="mt-3 text-sm font-semibold text-gray-900 xl:mt-2 xl:text-xs 2xl:mt-3 2xl:text-sm dark:text-white">
                {searchQuery || hasAdvancedFilters ? "No images found" : "No images in this category"}
              </h3>
              <p className="mt-1 max-w-sm text-center text-xs text-gray-600 xl:text-[10px] 2xl:text-xs dark:text-gray-400">
                {searchQuery || hasAdvancedFilters
                  ? "Try adjusting your filters or search query"
                  : "No images with this status yet"}
              </p>
            </div>
          </Card>
        )}
      </div>
    </div>
  );
};
