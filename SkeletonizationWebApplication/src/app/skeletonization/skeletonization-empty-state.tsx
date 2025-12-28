"use client";

import { useRouter } from "next/navigation";
import { Image as ImageIcon, Upload } from "lucide-react";

import { EmptyState } from "@/components/empty-state";

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
