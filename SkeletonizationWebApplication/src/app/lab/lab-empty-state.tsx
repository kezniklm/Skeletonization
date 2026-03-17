/**
 * @file lab-empty-state.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Renders empty states for processing lab route.
 * @description Displays contextual guidance when no images exist or no runs have been created.
 * @version 1.0
 * @date 2026-03-16
 */

"use client";

import { useRouter } from "next/navigation";
import { FlaskConical, Images } from "lucide-react";

import { EmptyState } from "@/components/empty-state";

/**
 * @brief Represents empty-state component properties.
 */
type LabEmptyStateProps = {
  hasImages: boolean;
};

/**
 * @brief Renders contextual empty-state card for lab page.
 * @param hasImages Whether user has uploaded images.
 * @returns Empty-state JSX.
 */
export const LabEmptyState = ({ hasImages }: LabEmptyStateProps) => {
  const router = useRouter();

  if (!hasImages) {
    return (
      <EmptyState
        icon={Images}
        title="No images uploaded"
        description="You need to upload images before you can start skeletonization runs. Upload your images to get started."
        actionLabel="Go to Images"
        actionIcon={Images}
        onAction={() => router.push("/images")}
        helpText="Upload at least one image to create skeletonization runs"
      />
    );
  }

  return (
    <EmptyState
      icon={FlaskConical}
      title="No runs yet"
      description="You haven't created any skeletonization runs yet. Start your first run to see processing results here."
      actionLabel="Start Skeletonization"
      actionIcon={FlaskConical}
      onAction={() => router.push("/skeletonization")}
      helpText="Select images and algorithms to begin processing"
    />
  );
};
