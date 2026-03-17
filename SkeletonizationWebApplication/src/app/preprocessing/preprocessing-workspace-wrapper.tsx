/**
 * @file preprocessing-workspace-wrapper.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Coordinates OpenCV and image loading before rendering preprocessing workspace.
 * @description Manages loading and error states while initializing runtime dependencies required by the workspace.
 */

"use client";

import { Loader2 } from "lucide-react";
import { useEffect } from "react";

import { Alert, AlertDescription } from "@/components/ui/alert";
import { type FileOutputFormat, type SelectImage } from "@/database/zod";

import { useOpenCV } from "./(hooks)/use-opencv";
import { useImageLoader } from "./(hooks)/use-image-loader";
import { PreprocessingWorkspace } from "./preprocessing-workspace";

type PreprocessingWorkspaceWrapperProps = {
  selectedImage: SelectImage;
  defaultOutputFormat: FileOutputFormat;
};

/**
 * @brief Initializes preprocessing prerequisites and renders the appropriate UI state.
 * @description Combines OpenCV initialization and image loading hooks, then displays loading, error, or ready workspace content.
 * @param selectedImage The image selected for preprocessing operations.
 * @param defaultOutputFormat User-preferred output format for saving processed images.
 * @returns A client component wrapping preprocessing workspace readiness states.
 */
export const PreprocessingWorkspaceWrapper = ({
  selectedImage,
  defaultOutputFormat
}: PreprocessingWorkspaceWrapperProps) => {
  const { status: openCvStatus, error: openCvError, cv } = useOpenCV({ timeoutMs: 10000 });
  const { status: imageLoadingStatus, image, errorMessage: imageLoadingError } = useImageLoader(selectedImage.url);

  useEffect(() => {
    document.body.style.overflow = "hidden";
    return () => {
      document.body.style.overflow = "";
    };
  }, []);

  if (openCvStatus === "loading" || imageLoadingStatus === "loading" || !image) {
    return (
      <div className="relative z-10 flex min-h-[60vh] flex-col items-center justify-center space-y-6 p-8">
        <div className="relative">
          <div className="absolute inset-0 animate-pulse rounded-full bg-cyan-500/20 blur-xl" />
          <Loader2 className="relative size-16 animate-spin text-cyan-500" />
        </div>
        <div className="space-y-2 text-center">
          <h3 className="text-xl font-semibold">Preparing workspace</h3>
          <p className="text-muted-foreground text-sm">Initializing OpenCV.js and loading your image...</p>
        </div>
        <div className="flex gap-2">
          <div className="h-2 w-2 animate-bounce rounded-full bg-cyan-500 [animation-delay:-0.3s]" />
          <div className="h-2 w-2 animate-bounce rounded-full bg-cyan-500 [animation-delay:-0.15s]" />
          <div className="h-2 w-2 animate-bounce rounded-full bg-cyan-500" />
        </div>
      </div>
    );
  }

  if (openCvStatus === "error" || imageLoadingStatus === "error") {
    const message = openCvError ?? imageLoadingError ?? "Failed to initialize preprocessing workspace";

    return (
      <div className="relative z-10 mx-auto max-w-4xl px-4 sm:px-6 lg:px-0 2xl:max-w-5xl">
        <Alert variant="destructive">
          <AlertDescription>{message}</AlertDescription>
        </Alert>
      </div>
    );
  }

  return (
    <PreprocessingWorkspace
      selectedImage={selectedImage}
      cv={cv}
      originalImage={image}
      defaultOutputFormat={defaultOutputFormat}
    />
  );
};
