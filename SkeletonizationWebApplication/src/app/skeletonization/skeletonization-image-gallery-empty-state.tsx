"use client";

import { Card, CardContent } from "@/components/ui/card";
import { type FilterType } from "@/components/filters";

type SkeletonizationImageGalleryEmptyStateProps = {
  selectedFilter: FilterType;
};

const getTitle = (selectedFilter: FilterType) => {
  switch (selectedFilter) {
    case "uploaded":
      return "No uploaded images";
    case "derived":
      return "No derived images";
    case "archived":
      return "No archived images";
    case "skeletonized":
      return "No skeletonized images";
    default:
      return "No images";
  }
};

const getDescription = (selectedFilter: FilterType) => {
  switch (selectedFilter) {
    case "uploaded":
      return "No uploaded images match your current filters.";
    case "derived":
      return "No derived images match your current filters.";
    default:
      return "No images match your current filters.";
  }
};

export const SkeletonizationImageGalleryEmptyState = ({
  selectedFilter
}: SkeletonizationImageGalleryEmptyStateProps) => {
  const title = getTitle(selectedFilter);
  const description = getDescription(selectedFilter);

  return (
    <Card className="border-2 border-dashed border-cyan-300 bg-linear-to-br from-cyan-50/50 to-blue-50/50 shadow-sm backdrop-blur-sm transition-all duration-300 hover:border-cyan-400 hover:shadow-xl dark:border-cyan-700 dark:from-cyan-950/20 dark:to-blue-950/20 dark:hover:border-cyan-600">
      <CardContent className="flex flex-col items-center justify-center space-y-2 py-8 text-center">
        <p className="text-sm font-semibold text-gray-900 dark:text-white">{title}</p>
        <p className="text-sm text-gray-600 dark:text-gray-400">{description}</p>
      </CardContent>
    </Card>
  );
};
