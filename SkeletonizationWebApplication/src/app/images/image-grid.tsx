"use client";

import { ImageIcon } from "lucide-react";

import type { SelectImage } from "@/database/zod/image";
import { Card, CardContent } from "@/components/ui/card";

import { ImageUpload } from "./image-upload";
import { ImageCard } from "./image-card";

type ImageGridProps = {
  images: SelectImage[];
  filteredImages: SelectImage[];
  searchQuery: string;
  hasAdvancedFilters: boolean;
  onUploadComplete: (image: SelectImage) => void;
  onDelete: (imageId: string) => void;
  onRename: (imageId: string, newFilename: string) => void;
};

export const ImageGrid = ({
  images,
  filteredImages,
  searchQuery,
  hasAdvancedFilters,
  onUploadComplete,
  onDelete,
  onRename
}: ImageGridProps) => {
  const showUploadCard = filteredImages.length > 0 || (!searchQuery && !hasAdvancedFilters);

  const showEmptyState = filteredImages.length === 0 && (searchQuery || hasAdvancedFilters);

  return (
    <div className="mt-6 space-y-4">
      {filteredImages.length > 0 && (
        <div className="text-muted-foreground flex items-center gap-2 text-sm">
          <div className="bg-primary h-1 w-1 rounded-full" />
          Showing <span className="text-foreground font-semibold">{filteredImages.length}</span> of{" "}
          <span className="text-foreground font-semibold">{images.length}</span> images
        </div>
      )}

      <div className="grid grid-cols-1 gap-6 sm:grid-cols-2">
        {showUploadCard && (
          <Card
            className={`group overflow-hidden border-2 border-dashed border-cyan-300 bg-linear-to-br from-cyan-50/50 to-blue-50/50 shadow-sm backdrop-blur-sm transition-all duration-300 hover:border-cyan-400 hover:shadow-lg dark:border-cyan-700 dark:from-cyan-950/20 dark:to-blue-950/20 dark:hover:border-cyan-600 ${filteredImages.length === 0 ? "sm:col-span-2" : ""}`}
          >
            <ImageUpload onUploadComplete={onUploadComplete} compact fullWidth={filteredImages.length === 0} />
          </Card>
        )}

        {filteredImages.map((image) => (
          <ImageCard key={image.id} image={image} onDelete={onDelete} onRename={onRename} />
        ))}
      </div>

      {showEmptyState && (
        <Card className="border-dashed border-gray-300 bg-white backdrop-blur-sm dark:border-gray-700 dark:bg-gray-900/95">
          <CardContent className="flex flex-col items-center justify-center py-20">
            <div className="bg-muted/50 rounded-full p-6">
              <ImageIcon className="text-muted-foreground/50 h-16 w-16" />
            </div>
            <h3 className="mt-6 text-xl font-semibold">No images found</h3>
            <p className="text-muted-foreground mt-2 max-w-sm text-center text-sm">
              Try adjusting your filters or search query to find what you&apos;re looking for
            </p>
          </CardContent>
        </Card>
      )}
    </div>
  );
};
