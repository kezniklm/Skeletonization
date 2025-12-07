"use client";

import { useRef, useState, type ChangeEvent } from "react";
import { useRouter } from "next/navigation";
import { FolderOpen, Image as ImageIcon, Loader2, Upload } from "lucide-react";

import { Button } from "@/components/ui/button";
import { Card, CardContent } from "@/components/ui/card";
import {
  AlertDialog,
  AlertDialogAction,
  AlertDialogCancel,
  AlertDialogContent,
  AlertDialogDescription,
  AlertDialogFooter,
  AlertDialogHeader,
  AlertDialogTitle
} from "@/components/ui/alert-dialog";
import { Input } from "@/components/ui/input";
import { Label } from "@/components/ui/label";
import { uploadImageAction } from "@/server-actions/images";
import { generateRandomFilename } from "@/lib/files";

type PreprocessingEmptyStateProps = {
  availableImagesCount: number;
};

export const PreprocessingSelectImage = ({ availableImagesCount }: PreprocessingEmptyStateProps) => {
  const router = useRouter();
  const fileInputRef = useRef<HTMLInputElement>(null);

  const [uploading, setUploading] = useState(false);
  const [uploadProgress, setUploadProgress] = useState(0);

  const [conflictDialogOpen, setConflictDialogOpen] = useState(false);
  const [conflictFile, setConflictFile] = useState<File | null>(null);
  const [conflictMessage, setConflictMessage] = useState("");
  const [newFileName, setNewFileName] = useState("");

  const isNameConflictError = (message: string) => /already.*(exists|uploaded)|conflict/i.test(message);

  const startFakeProgress = () => {
    setUploadProgress(0);
    const interval = setInterval(() => {
      setUploadProgress((prev) => {
        if (prev >= 90) return prev;
        return prev + 10;
      });
    }, 100);
    return interval;
  };

  const resetFileInput = () => {
    if (fileInputRef.current) {
      fileInputRef.current.value = "";
    }
  };

  const handleFileUpload = async (event: ChangeEvent<HTMLInputElement>) => {
    const file = event.target.files?.[0];
    if (!file) return;

    setUploading(true);
    const progressInterval = startFakeProgress();

    try {
      const uploadedImage = await uploadImageAction(file);
      clearInterval(progressInterval);
      setUploadProgress(100);
      router.push(`/preprocessing?imageId=${uploadedImage.id}`);
    } catch (err) {
      clearInterval(progressInterval);
      setUploadProgress(0);

      const message = err instanceof Error ? err.message : "Failed to upload image";

      if (isNameConflictError(message)) {
        setConflictFile(file);
        setNewFileName(generateRandomFilename(file.name));
        setConflictMessage("An image with this name (or content) already exists. Please choose a different filename.");
        setConflictDialogOpen(true);
      } else {
        setConflictFile(null);
        setConflictMessage(message);
        setConflictDialogOpen(true);
      }
    } finally {
      setUploading(false);
      resetFileInput();
      setUploadProgress(0);
    }
  };

  const handleConflictRetry = async () => {
    if (!conflictFile || !newFileName.trim()) {
      setConflictDialogOpen(false);
      return;
    }

    setConflictDialogOpen(false);
    setUploading(true);
    const progressInterval = startFakeProgress();

    try {
      const renamedFile = new File([conflictFile], newFileName.trim(), {
        type: conflictFile.type
      });

      const uploadedImage = await uploadImageAction(renamedFile);
      clearInterval(progressInterval);
      setUploadProgress(100);
      router.push(`/preprocessing?imageId=${uploadedImage.id}`);
    } catch (err) {
      clearInterval(progressInterval);
      setUploadProgress(0);

      const message = err instanceof Error ? err.message : "Failed to upload image";
      setConflictFile(null);
      setConflictMessage(message);
      setConflictDialogOpen(true);
    } finally {
      setUploading(false);
      setUploadProgress(0);
      setConflictFile(null);
      resetFileInput();
    }
  };

  const handlePickFile = () => {
    if (!uploading) {
      fileInputRef.current?.click();
    }
  };

  const onBrowseGallery = () => {
    router.push("/images");
  };

  return (
    <div className="relative z-10 mx-auto max-w-4xl px-4 sm:px-6 lg:px-0 2xl:max-w-5xl">
      <div className="mb-8">
        <h1 className="bg-linear-to-r from-cyan-600 to-blue-600 bg-clip-text text-4xl font-bold text-transparent xl:text-3xl 2xl:text-4xl dark:from-cyan-400 dark:to-blue-400">
          Image Preprocessing
        </h1>
        <p className="mt-2 text-gray-600 xl:text-sm 2xl:text-base dark:text-gray-400">
          Apply filters, transformations, and advanced OpenCV operations to your images
        </p>
      </div>

      <AlertDialog open={conflictDialogOpen} onOpenChange={setConflictDialogOpen}>
        <AlertDialogContent>
          <AlertDialogHeader>
            <AlertDialogTitle>{conflictFile ? "Filename Conflict" : "Upload Error"}</AlertDialogTitle>
            <AlertDialogDescription>{conflictMessage}</AlertDialogDescription>
          </AlertDialogHeader>

          {conflictFile && (
            <div className="space-y-3">
              <Label htmlFor="new-filename">New filename</Label>
              <div className="flex flex-col gap-2 sm:flex-row">
                <Input
                  id="new-filename"
                  value={newFileName}
                  onChange={(e) => setNewFileName(e.target.value)}
                  placeholder="Enter new filename"
                  onKeyDown={(e) => {
                    if (e.key === "Enter" && newFileName.trim()) {
                      handleConflictRetry();
                    }
                  }}
                />
                <Button
                  type="button"
                  variant="outline"
                  onClick={() => conflictFile && setNewFileName(generateRandomFilename(conflictFile.name))}
                >
                  Generate name
                </Button>
              </div>
            </div>
          )}

          <AlertDialogFooter>
            <AlertDialogCancel>Close</AlertDialogCancel>
            {conflictFile && (
              <AlertDialogAction onClick={handleConflictRetry} disabled={!newFileName.trim()}>
                Upload with new name
              </AlertDialogAction>
            )}
          </AlertDialogFooter>
        </AlertDialogContent>
      </AlertDialog>

      <input
        ref={fileInputRef}
        type="file"
        accept="image/png,image/jpeg,image/jpg,image/bmp,image/tiff"
        onChange={handleFileUpload}
        className="hidden"
      />

      {availableImagesCount === 0 ? (
        <Card className="border-2 border-dashed border-cyan-300 bg-linear-to-br from-cyan-50/50 to-blue-50/50 shadow-sm backdrop-blur-sm transition-all duration-300 hover:border-cyan-400 hover:shadow-xl dark:border-cyan-700 dark:from-cyan-950/20 dark:to-blue-950/20 dark:hover:border-cyan-600">
          <CardContent className="flex min-h-[300px] flex-col items-center justify-center space-y-4 py-12">
            <div className="rounded-full bg-linear-to-r from-cyan-500 to-blue-500 p-4">
              <ImageIcon className="size-10 text-white" />
            </div>
            <div className="space-y-2 text-center">
              <h3 className="text-xl font-bold text-gray-900 dark:text-white">No Images Yet</h3>
              <p className="max-w-md text-sm text-gray-600 dark:text-gray-400">
                Upload your first image to start preprocessing with advanced filters and transformations
              </p>
            </div>
            <div className="flex flex-col gap-3 sm:flex-row">
              <Button onClick={handlePickFile} disabled={uploading}>
                {uploading ? (
                  <>
                    <Loader2 className="mr-2 size-4 animate-spin" />
                    Uploading...
                  </>
                ) : (
                  <>
                    <Upload className="mr-2 size-5" />
                    Upload Image
                  </>
                )}
              </Button>
            </div>
            <p className="text-xs text-gray-500 dark:text-gray-500">PNG, JPEG, BMP, TIFF • Max 10MB</p>
          </CardContent>
        </Card>
      ) : (
        <div className="grid gap-4 sm:grid-cols-2 xl:gap-6">
          <Card className="group overflow-hidden border-2 border-dashed border-cyan-300 bg-linear-to-br from-cyan-50/50 to-blue-50/50 shadow-sm backdrop-blur-sm transition-all duration-300 hover:border-cyan-400 hover:shadow-xl dark:border-cyan-700 dark:from-cyan-950/20 dark:to-blue-950/20 dark:hover:border-cyan-600">
            <button
              type="button"
              onClick={handlePickFile}
              className="relative flex aspect-4/3 w-full flex-col items-center justify-center text-center transition-all focus:outline-none focus-visible:ring-2 focus-visible:ring-cyan-500 sm:aspect-square lg:aspect-4/3"
            >
              {uploading ? (
                <div className="w-full space-y-3 px-4">
                  <div className="mx-auto rounded-full bg-cyan-100 p-3 dark:bg-cyan-900/30">
                    <Upload className="size-6 text-cyan-600 dark:text-cyan-400" />
                  </div>
                  <p className="text-sm font-medium text-gray-900 dark:text-white">Uploading... {uploadProgress}%</p>
                  <div className="mx-auto h-2 w-full max-w-xs overflow-hidden rounded-full bg-gray-200 dark:bg-gray-700">
                    <div
                      className="h-full bg-linear-to-r from-cyan-500 to-blue-500 transition-all duration-300"
                      style={{ width: `${uploadProgress}%` }}
                    />
                  </div>
                </div>
              ) : (
                <>
                  <div className="mb-2 rounded-full bg-linear-to-r from-cyan-500 to-blue-500 p-3">
                    <Upload className="size-6 text-white" />
                  </div>
                  <p className="text-sm font-semibold text-gray-900 dark:text-white">Upload Image</p>
                  <p className="mt-1 text-xs text-gray-600 dark:text-gray-400">Click to select a file</p>
                  <p className="mt-1 text-xs text-gray-500 dark:text-gray-500">PNG, JPEG, BMP, TIFF</p>
                </>
              )}
            </button>
          </Card>

          <Card className="group overflow-hidden border-cyan-300 bg-linear-to-br from-cyan-50/50 to-blue-50/50 shadow-sm backdrop-blur-sm transition-all duration-300 hover:border-cyan-400 hover:shadow-xl dark:border-cyan-700 dark:from-cyan-950/20 dark:to-blue-950/20 dark:hover:border-cyan-600">
            <button
              type="button"
              onClick={onBrowseGallery}
              className="relative flex aspect-4/3 w-full flex-col items-center justify-center text-center transition-all focus:outline-none focus-visible:ring-2 focus-visible:ring-cyan-500 sm:aspect-square lg:aspect-4/3"
            >
              <div className="mb-2 rounded-full bg-linear-to-r from-cyan-500 to-blue-500 p-3">
                <FolderOpen className="size-6 text-white" />
              </div>
              <p className="text-sm font-semibold text-gray-900 dark:text-white">Browse Gallery</p>
              <p className="mt-1 text-xs text-gray-600 dark:text-gray-400">
                {availableImagesCount} {availableImagesCount === 1 ? "image" : "images"} available
              </p>
              <p className="mt-1 text-xs text-gray-500 dark:text-gray-500">View all your uploaded images</p>
            </button>
          </Card>
        </div>
      )}
    </div>
  );
};
