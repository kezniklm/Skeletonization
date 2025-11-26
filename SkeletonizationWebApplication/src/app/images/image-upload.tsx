"use client";

import { Upload, X } from "lucide-react";
import { useCallback, useState } from "react";
import { toast } from "sonner";

import type { SelectImage } from "@/database/zod/image";
import { uploadImageAction } from "@/server-actions/images";
import { Button } from "@/components/ui/button";
import { Card, CardContent } from "@/components/ui/card";

type ImageUploadProps = {
  onUploadComplete: (image: SelectImage) => void;
  compact?: boolean;
  fullWidth?: boolean;
};

export const ImageUpload = ({ onUploadComplete, compact = false, fullWidth = false }: ImageUploadProps) => {
  const [isDragging, setIsDragging] = useState(false);
  const [isUploading, setIsUploading] = useState(false);
  const [uploadProgress, setUploadProgress] = useState(0);
  const [totalFiles, setTotalFiles] = useState(0);
  const [uploadedFiles, setUploadedFiles] = useState(0);

  const uploadFile = useCallback(
    async (file: File) => {
      try {
        const image = await uploadImageAction(file);
        onUploadComplete(image as SelectImage);
        setUploadedFiles((prev) => prev + 1);
        return { success: true, file };
      } catch (error) {
        console.error("Upload error:", error);
        const errorMessage = error instanceof Error ? error.message : "Unknown error";
        toast.error(`Failed to upload "${file.name}": ${errorMessage}`);
        return { success: false, file, error: errorMessage };
      }
    },
    [onUploadComplete]
  );

  const uploadFiles = useCallback(
    async (files: File[]) => {
      if (files.length === 0) return;

      setIsUploading(true);
      setUploadProgress(0);
      setTotalFiles(files.length);
      setUploadedFiles(0);

      const progressInterval = setInterval(() => {
        setUploadProgress((prev) => {
          const target = (uploadedFiles / files.length) * 90;
          return Math.min(prev + 5, target);
        });
      }, 100);

      const results = await Promise.allSettled(files.map((file) => uploadFile(file)));

      clearInterval(progressInterval);
      setUploadProgress(100);

      const successCount = results.filter((r) => r.status === "fulfilled" && r.value.success).length;
      const failureCount = files.length - successCount;

      if (failureCount === 0) {
        toast.success(
          files.length === 1 ? "Image uploaded successfully!" : `All ${files.length} images uploaded successfully!`
        );
      } else if (successCount === 0) {
        toast.error(files.length === 1 ? "Failed to upload image" : `Failed to upload all ${files.length} images`);
      } else {
        toast.warning(
          `${successCount} of ${files.length} images uploaded successfully. ${failureCount} failed to upload.`
        );
      }

      setTimeout(() => {
        setIsUploading(false);
        setUploadProgress(0);
        setTotalFiles(0);
        setUploadedFiles(0);
      }, 500);
    },
    [uploadFile, uploadedFiles]
  );

  const handleDrop = useCallback(
    (e: React.DragEvent) => {
      e.preventDefault();
      setIsDragging(false);

      const files = Array.from(e.dataTransfer.files);
      const imageFiles = files.filter((file) => file.type.startsWith("image/"));

      if (imageFiles.length === 0) {
        toast.error("Please drop image files only");
        return;
      }

      uploadFiles(imageFiles);
    },
    [uploadFiles]
  );

  const handleDragOver = useCallback((e: React.DragEvent) => {
    e.preventDefault();
    setIsDragging(true);
  }, []);

  const handleDragLeave = useCallback((e: React.DragEvent) => {
    e.preventDefault();
    setIsDragging(false);
  }, []);

  const handleFileSelect = (e: React.ChangeEvent<HTMLInputElement>) => {
    const files = e.target.files;
    if (files && files.length > 0) {
      uploadFiles(Array.from(files));
    }
  };

  if (compact) {
    return (
      <CardContent className="flex h-full items-center justify-center p-0">
        <div
          onDrop={handleDrop}
          onDragOver={handleDragOver}
          onDragLeave={handleDragLeave}
          className={`relative flex w-full cursor-pointer flex-col items-center justify-center rounded-lg transition-all ${fullWidth ? "py-16" : "aspect-video"} ${
            isUploading ? "pointer-events-none opacity-50" : ""
          }`}
        >
          <input
            type="file"
            id="image-upload-compact"
            accept="image/png,image/jpeg,image/jpg,image/bmp,image/tiff"
            onChange={handleFileSelect}
            disabled={isUploading}
            multiple
            className="hidden"
          />

          <label
            htmlFor="image-upload-compact"
            className={`flex h-full w-full cursor-pointer flex-col items-center justify-center text-center ${fullWidth ? "px-4 py-8" : "px-3 py-4"}`}
          >
            {isUploading ? (
              <div className={`w-full space-y-3 ${fullWidth ? "px-4" : "px-2"}`}>
                <div className={`mx-auto rounded-full bg-cyan-100 p-3 dark:bg-cyan-900/30 ${fullWidth ? "" : "p-2"}`}>
                  <Upload className={`text-cyan-600 dark:text-cyan-400 ${fullWidth ? "h-8 w-8" : "h-6 w-6"}`} />
                </div>
                <p className={`font-medium text-gray-900 dark:text-white ${fullWidth ? "text-sm" : "text-xs"}`}>
                  Uploading...
                </p>
                <div className="mx-auto h-2 w-full max-w-xs overflow-hidden rounded-full bg-gray-200 dark:bg-gray-700">
                  <div
                    className="h-full bg-linear-to-r from-cyan-500 to-blue-500 transition-all duration-300"
                    style={{ width: `${uploadProgress}%` }}
                  />
                </div>
                <p className="text-xs text-gray-600 dark:text-gray-400">
                  {totalFiles > 1 ? `${uploadedFiles} of ${totalFiles} files` : `${uploadProgress}%`}
                </p>
              </div>
            ) : (
              <>
                <div
                  className={`rounded-full bg-linear-to-r from-cyan-500 to-blue-500 ${fullWidth ? "mb-3 p-4" : "mb-2 p-3"}`}
                >
                  <Upload className={`text-white ${fullWidth ? "h-8 w-8" : "h-5 w-5"}`} />
                </div>
                <p className={`font-semibold text-gray-900 dark:text-white ${fullWidth ? "text-sm" : "text-xs"}`}>
                  Upload Images
                </p>
                <p className={`mt-1 text-gray-600 dark:text-gray-400 ${fullWidth ? "text-xs" : "text-[10px]"}`}>
                  Click or drag & drop
                </p>
                {fullWidth && (
                  <p className="mt-1 text-xs text-gray-500 dark:text-gray-500">PNG, JPEG, BMP, TIFF • Multiple files</p>
                )}
              </>
            )}
          </label>

          {isDragging && (
            <div className="pointer-events-none absolute inset-0 flex items-center justify-center rounded-lg bg-cyan-500/20">
              <div className="rounded-full bg-linear-to-r from-cyan-500 to-blue-500 p-4 text-white shadow-lg">
                <Upload className="h-8 w-8" />
              </div>
            </div>
          )}
        </div>
      </CardContent>
    );
  }

  return (
    <Card className="border-gray-200 bg-white shadow-sm backdrop-blur-sm dark:border-gray-800 dark:bg-gray-900/95">
      <CardContent className="p-6">
        <div
          onDrop={handleDrop}
          onDragOver={handleDragOver}
          onDragLeave={handleDragLeave}
          className={`relative rounded-lg border-2 border-dashed transition-all ${
            isDragging
              ? "border-cyan-500 bg-cyan-50 dark:border-cyan-400 dark:bg-cyan-950/20"
              : "border-gray-300 bg-gray-50 hover:border-cyan-400 hover:bg-cyan-50/50 dark:border-gray-700 dark:bg-gray-800/50 dark:hover:border-cyan-500 dark:hover:bg-cyan-950/10"
          } ${isUploading ? "pointer-events-none opacity-50" : ""}`}
        >
          <input
            type="file"
            id="image-upload"
            accept="image/png,image/jpeg,image/jpg,image/bmp,image/tiff"
            onChange={handleFileSelect}
            disabled={isUploading}
            multiple
            className="hidden"
          />

          <label
            htmlFor="image-upload"
            className="flex cursor-pointer flex-col items-center justify-center px-6 py-12 text-center"
          >
            <div
              className={`mb-4 rounded-full p-3 transition-colors ${
                isDragging ? "bg-cyan-100 dark:bg-cyan-900/30" : "bg-gray-100 dark:bg-gray-800"
              }`}
            >
              <Upload
                className={`h-8 w-8 ${isDragging ? "text-cyan-600 dark:text-cyan-400" : "text-gray-400 dark:text-gray-500"}`}
              />
            </div>

            {isUploading ? (
              <div className="w-full max-w-xs space-y-3">
                <p className="text-sm font-medium text-gray-900 dark:text-white">
                  {totalFiles > 1 ? `Uploading ${totalFiles} files...` : "Uploading..."}
                </p>
                <div className="h-2 w-full overflow-hidden rounded-full bg-gray-200 dark:bg-gray-700">
                  <div
                    className="h-full bg-linear-to-r from-cyan-500 to-blue-500 transition-all duration-300"
                    style={{ width: `${uploadProgress}%` }}
                  />
                </div>
                <p className="text-xs text-gray-600 dark:text-gray-400">
                  {totalFiles > 1 ? `${uploadedFiles} of ${totalFiles} completed` : `${uploadProgress}%`}
                </p>
              </div>
            ) : (
              <>
                <p className="mb-2 text-sm font-medium text-gray-900 dark:text-white">
                  <span className="font-semibold text-cyan-600 dark:text-cyan-400">Click to upload</span> or drag and
                  drop
                </p>
                <p className="text-xs text-gray-600 dark:text-gray-400">
                  PNG, JPEG, BMP, or TIFF • Multiple files supported (max 10MB each)
                </p>
              </>
            )}
          </label>

          {isDragging && (
            <div className="pointer-events-none absolute inset-0 flex items-center justify-center rounded-lg bg-cyan-500/10">
              <div className="rounded-full bg-linear-to-r from-cyan-500 to-blue-500 p-4 text-white shadow-lg">
                <Upload className="h-8 w-8" />
              </div>
            </div>
          )}
        </div>

        {isUploading && (
          <Button
            variant="outline"
            onClick={() => {
              setIsUploading(false);
              setUploadProgress(0);
              toast.info("Upload cancelled");
            }}
            className="mt-4 w-full gap-2"
          >
            <X className="h-4 w-4" />
            Cancel Upload
          </Button>
        )}
      </CardContent>
    </Card>
  );
};
