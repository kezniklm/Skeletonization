"use client";

import { Upload, X } from "lucide-react";
import { useState } from "react";
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

  const notifyUploadSummary = (files: File[], successCount: number, failureCount: number) => {
    if (files.length === 1) {
      if (successCount === 1) {
        toast.success(`Image ${files[0]?.name ?? ""} uploaded successfully!`);
      }
      return;
    }

    if (failureCount === 0) {
      toast.success("Images uploaded successfully!");
      return;
    }

    if (successCount === 0) {
      toast.error(`All ${files.length} uploads failed`);
      return;
    }

    toast.info(`${successCount} uploaded, ${failureCount} failed`);
  };

  const uploadFile = async (file: File, totalCount: number) => {
    try {
      const image = await uploadImageAction(file);
      onUploadComplete(image);
      return { success: true };
    } catch (error) {
      const message = error instanceof Error ? error.message : "Unknown error";
      toast.error(`Failed to upload ${file.name}: ${message}`);
      return { success: false };
    } finally {
      setUploadedFiles((prev) => {
        const next = prev + 1;
        const progress = totalCount > 0 ? Math.round((next / totalCount) * 100) : 0;
        setUploadProgress(progress);
        return next;
      });
    }
  };

  const uploadFiles = async (files: File[]) => {
    if (!files.length) return;
    setIsUploading(true);
    setTotalFiles(files.length);
    setUploadedFiles(0);

    const results = await Promise.all(files.map((f) => uploadFile(f, files.length)));
    const successCount = results.filter((r) => r.success).length;
    const failureCount = results.length - successCount;

    notifyUploadSummary(files, successCount, failureCount);

    setIsUploading(false);
    setUploadProgress(0);
  };

  const handleDrop = (e: React.DragEvent) => {
    e.preventDefault();
    setIsDragging(false);
    const files = Array.from(e.dataTransfer.files).filter((f) => f.type.startsWith("image/"));
    if (!files.length) return toast.error("Please drop image files only");
    uploadFiles(files);
  };

  const handleDragOver = (e: React.DragEvent) => {
    e.preventDefault();
    setIsDragging(true);
  };
  const handleDragLeave = (e: React.DragEvent) => {
    e.preventDefault();
    setIsDragging(false);
  };
  const handleFileSelect = (e: React.ChangeEvent<HTMLInputElement>) => {
    const files = e.target.files ? Array.from(e.target.files) : [];
    if (files.length) uploadFiles(files);
  };

  if (compact) {
    const wrapperClass = fullWidth ? "aspect-[64/27]" : "h-full";

    return (
      <Card
        className={`group overflow-hidden border-2 border-dashed border-cyan-300 bg-linear-to-br from-cyan-50/50 to-blue-50/50 shadow-sm backdrop-blur-sm transition-all duration-300 hover:border-cyan-400 hover:shadow-lg dark:border-cyan-700 dark:from-cyan-950/20 dark:to-blue-950/20 dark:hover:border-cyan-600 ${
          fullWidth ? "sm:col-span-2" : ""
        }`}
      >
        <div
          onDrop={handleDrop}
          onDragOver={handleDragOver}
          onDragLeave={handleDragLeave}
          className={`relative flex w-full items-center justify-center ${wrapperClass} ${
            isUploading ? "pointer-events-none opacity-50" : ""
          }`}
        >
          <input
            id="image-upload-compact"
            type="file"
            accept="image/png,image/jpeg,image/jpg,image/bmp,image/tiff"
            multiple
            disabled={isUploading}
            onChange={handleFileSelect}
            className="hidden"
          />

          <label
            htmlFor="image-upload-compact"
            className="flex h-full w-full flex-col items-center justify-center gap-2 text-center"
          >
            {isUploading ? (
              <div className="w-full max-w-xs space-y-3">
                <div className="mx-auto rounded-full bg-cyan-100 p-3 dark:bg-cyan-900/30">
                  <Upload className="h-6 w-6 text-cyan-600 dark:text-cyan-400" />
                </div>
                <p className="text-xs font-medium text-gray-900 dark:text-white">
                  {totalFiles > 1 ? `Uploading ${totalFiles} files...` : "Uploading..."}
                </p>
                <div className="mx-auto h-2 w-full overflow-hidden rounded-full bg-gray-200 dark:bg-gray-700">
                  <div
                    className="h-full bg-linear-to-r from-cyan-500 to-blue-500 transition-all duration-300"
                    style={{ width: `${uploadProgress}%` }}
                  />
                </div>
                <p className="text-[11px] text-gray-600 dark:text-gray-400">
                  {uploadedFiles}/{totalFiles} done
                </p>
              </div>
            ) : (
              <>
                <div className="mb-1.5 rounded-full bg-linear-to-r from-cyan-500 to-blue-500 p-3">
                  <Upload className="h-5 w-5 text-white" />
                </div>
                <p className="text-xs font-semibold text-gray-900 dark:text-white">Upload Images</p>
                <p className="text-[10px] text-gray-600 dark:text-gray-400">Click or drag & drop</p>
                {fullWidth && (
                  <p className="text-[11px] text-gray-500 dark:text-gray-500">PNG, JPEG, BMP, TIFF • Multiple files</p>
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
      </Card>
    );
  }

  return (
    <Card className="border-gray-200 bg-white shadow-sm backdrop-blur-sm xl:rounded-lg dark:border-gray-800 dark:bg-gray-900/95">
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
            id="image-upload"
            type="file"
            accept="image/png,image/jpeg,image/jpg,image/bmp,image/tiff"
            multiple
            disabled={isUploading}
            onChange={handleFileSelect}
            className="hidden"
          />

          <label htmlFor="image-upload" className="flex flex-col items-center justify-center px-6 py-12 text-center">
            <div
              className={`mb-4 rounded-full p-3 ${
                isDragging ? "bg-cyan-100 dark:bg-cyan-900/30" : "bg-gray-100 dark:bg-gray-800"
              }`}
            >
              <Upload
                className={`h-8 w-8 ${
                  isDragging ? "text-cyan-600 dark:text-cyan-400" : "text-gray-400 dark:text-gray-500"
                }`}
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
              </div>
            ) : (
              <>
                <p className="mb-2 text-sm font-medium text-gray-900 dark:text-white">
                  <span className="font-semibold text-cyan-600 dark:text-cyan-400">Click to upload</span> or drag and
                  drop
                </p>
                <p className="text-xs text-gray-600 dark:text-gray-400">
                  PNG, JPEG, BMP, or TIFF • Multiple files supported
                </p>
              </>
            )}
          </label>
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
            <X className="h-4 w-4" /> Cancel Upload
          </Button>
        )}
      </CardContent>
    </Card>
  );
};
