/**
 * @file skeletonization-empty-state.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Displays empty state when no processable images exist.
 * @description Provides a call-to-action directing users to the image gallery to upload or prepare images.
 */

"use client";

import { useRouter } from "next/navigation";
import { Image as ImageIcon, Upload } from "lucide-react";

import { EmptyState } from "@/components/empty-state";

/**
 * @brief Renders the skeletonization page empty-state panel.
 * @description Shows guidance and a navigation action when there are no eligible images for skeletonization.
 * @returns A reusable empty-state component instance.
 */
export const SkeletonizationEmptyState = () => {
  const router = useRouter();

  const onBrowseGallery = () => {
    router.push("/images");
  };

  return (
    <EmptyState
      icon={ImageIcon}
      title="No Images Available"
      description="Upload and validate images to start batch skeletonization processing"
      actionLabel="Go to Image Gallery"
      actionIcon={Upload}
      onAction={onBrowseGallery}
      helpText="Only uploaded and derived images can be processed"
    />
  );
};
