/**
 * @file image-grid.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Renders grid layout of upload card and image cards.
 * @description Handles empty states and filtered-image presentation for current gallery tab selection.
 * @version 1.0
 * @date 2026-03-16
 */

"use client";

import { ImageIcon, Upload } from "lucide-react";
import { useRouter } from "next/navigation";

import type { SelectImage } from "@/database/zod/image";
import { EmptyState } from "@/components/empty-state";
import { capitalizeFirst } from "@/lib/format";

import { ImageUpload } from "./image-upload";
import { ImageCard } from "./image-card";

/**
 * @brief Represents image grid input properties.
 */
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

/**
 * @brief Renders filtered image grid with upload and empty-state handling.
 * @param images Full image collection.
 * @param filteredImages Image subset after filters and pagination.
 * @param searchQuery Active search query.
 * @param hasAdvancedFilters Whether non-search filters are active.
 * @param selectedFilter Active tab filter.
 * @param onUploadComplete Callback for successful upload.
 * @param onDelete Callback for image delete request.
 * @param onRename Callback for image rename request.
 * @param onArchive Callback for archive action.
 * @param onUnarchive Callback for unarchive action.
 * @returns Image grid JSX.
 */
/** @brief Renders image tiles, upload card, and filtered empty states. */
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
  const router = useRouter();
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
          <EmptyState
            icon={ImageIcon}
            title={
              searchQuery || hasAdvancedFilters
                ? "No images found"
                : selectedFilter === "all"
                  ? "No Images to Display"
                  : `No ${capitalizeFirst(selectedFilter)} Images`
            }
            description={
              searchQuery || hasAdvancedFilters
                ? "Try adjusting your filters or search query"
                : selectedFilter === "all"
                  ? "All your images are currently archived.\n Check the Archived tab to view them."
                  : `You don't have any images marked as ${selectedFilter} yet`
            }
            actionLabel="Upload Image"
            actionIcon={Upload}
            onAction={() => router.push("/preprocessing")}
            helpText={selectedFilter === "all" ? "Or upload a new image to get started" : undefined}
            className="aspect-64/27 sm:col-span-2"
          />
        )}
      </div>
    </div>
  );
};
