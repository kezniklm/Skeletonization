"use client";

import { useRouter } from "next/navigation";
import { FlaskConical, Images } from "lucide-react";

import { EmptyState } from "@/components/empty-state";

type LabEmptyStateProps = {
  hasImages: boolean;
};

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
