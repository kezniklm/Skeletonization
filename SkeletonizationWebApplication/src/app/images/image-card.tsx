/**
 * @file image-card.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Renders interactive card for one stored image.
 * @description Displays metadata and actions for processing, renaming, archiving, unarchiving, and deleting an image.
 * @version 1.0
 * @date 2026-03-16
 */

"use client";

import { Archive, ArchiveRestore, Calendar, Check, Eye, ImageIcon, Pencil, Trash2, X } from "lucide-react";
import Image from "next/image";
import Link from "next/link";
import { useState } from "react";
import { toast } from "sonner";

import type { SelectImage } from "@/database/zod/image";
import { updateImageAction } from "@/server-actions/images";
import { Badge } from "@/components/ui/badge";
import { Button } from "@/components/ui/button";
import { Card, CardContent } from "@/components/ui/card";
import { Input } from "@/components/ui/input";
import { useTimezone } from "@/contexts/timezone-context";

import { formatDate, formatFileSize, getStatusBadgeClass, getStatusLabel } from "./utils";

/**
 * @brief Represents image card action handlers and payload.
 */
type ImageCardProps = {
  image: SelectImage;
  onDelete: (imageId: string) => void;
  onRename: (imageId: string, newFilename: string) => void;
  onArchive: (imageId: string) => void;
  onUnarchive: (imageId: string) => void;
};

/**
 * @brief Renders image card with preview, metadata, and actions.
 * @param image Image entity to present.
 * @param onDelete Callback for delete request.
 * @param onRename Callback for filename update.
 * @param onArchive Callback for archive action.
 * @param onUnarchive Callback for unarchive action.
 * @returns Image card JSX.
 */
/** @brief Displays one image card with processing and management actions. */
export const ImageCard = ({ image, onDelete, onRename, onArchive, onUnarchive }: ImageCardProps) => {
  const { resolvedTimezone } = useTimezone();
  const [isEditing, setIsEditing] = useState(false);
  const [editedName, setEditedName] = useState("");
  const [isSaving, setIsSaving] = useState(false);

  const statusLabel = getStatusLabel(image.status);
  const statusClass = getStatusBadgeClass(image.status);
  const mimeLabel = image.mime.split("/")[1]?.toUpperCase() ?? "N/A";

  /**
   * @brief Removes extension from filename.
   * @param filename Input filename.
   * @returns Filename without extension segment.
   */
  const getFilenameWithoutExt = (filename: string) => {
    const lastDot = filename.lastIndexOf(".");
    return lastDot > 0 ? filename.substring(0, lastDot) : filename;
  };

  /**
   * @brief Extracts extension from filename.
   * @param filename Input filename.
   * @returns Extension including leading dot or empty string.
   */
  const getFileExtension = (filename: string) => {
    const lastDot = filename.lastIndexOf(".");
    return lastDot > 0 ? filename.substring(lastDot) : "";
  };

  /**
   * @brief Opens inline filename editor.
   * @returns No return value.
   */
  const startEditing = () => {
    setEditedName(getFilenameWithoutExt(image.originalFilename));
    setIsEditing(true);
  };

  /**
   * @brief Cancels inline edit state.
   * @returns No return value.
   */
  const cancelEditing = () => {
    setIsEditing(false);
    setEditedName("");
  };

  /**
   * @brief Persists updated filename.
   * @returns Promise resolved when save flow completes.
   */
  const saveFilename = async () => {
    const trimmedName = editedName.trim();
    if (!trimmedName) {
      toast.error("Filename cannot be empty");
      return;
    }

    const extension = getFileExtension(image.originalFilename);
    const newFilename = trimmedName + extension;

    if (newFilename === image.originalFilename) {
      cancelEditing();
      return;
    }

    setIsSaving(true);
    try {
      await updateImageAction(image.id, { originalFilename: newFilename });
      onRename(image.id, newFilename);
      toast.success("Filename updated");
      setIsEditing(false);
    } catch (error) {
      console.error("Rename error:", error);
      toast.error("Failed to rename image");
    } finally {
      setIsSaving(false);
    }
  };

  /**
   * @brief Handles keyboard shortcuts in rename input.
   * @param e Keyboard event from input field.
   * @returns No return value.
   */
  const handleKeyDown = (e: React.KeyboardEvent) => {
    if (e.key === "Enter") {
      saveFilename();
    } else if (e.key === "Escape") {
      cancelEditing();
    }
  };

  return (
    <Card className="group gap-1 overflow-hidden border-gray-200 bg-white pt-0 pb-2 shadow-sm backdrop-blur-sm transition-all duration-300 hover:border-cyan-300 hover:shadow-xl xl:shadow-sm 2xl:shadow-xl dark:border-gray-800 dark:bg-gray-900/95 dark:hover:border-cyan-700">
      <Link href={`/preprocessing?imageId=${image.id}`} className="block">
        <div className="from-muted/30 to-muted/60 relative aspect-video overflow-hidden bg-linear-to-br">
          <Image
            src={image.url}
            alt={image.originalFilename}
            fill
            className="object-contain transition-all duration-500 group-hover:scale-105"
            sizes="(max-width: 640px) 100vw, 50vw"
          />

          <div className="absolute inset-0 bg-linear-to-t from-black/70 via-black/20 to-transparent opacity-0 transition-opacity duration-300 group-hover:opacity-100" />

          <div className="absolute right-3 bottom-3 left-3 flex items-center justify-between opacity-0 transition-all duration-300 group-hover:opacity-100">
            <div className="flex items-center gap-2 rounded-lg bg-white/95 px-3 py-2 shadow-xl backdrop-blur-md dark:bg-gray-900/95">
              <Eye className="h-4 w-4 text-cyan-600 dark:text-cyan-400" />
              <span className="text-sm font-semibold text-gray-900 dark:text-white">View & Process</span>
            </div>
          </div>
        </div>
      </Link>

      <CardContent className="space-y-2.5 p-3 xl:space-y-2 xl:p-2.5 2xl:space-y-2.5 2xl:p-3">
        <div className="space-y-2 xl:space-y-1.5 2xl:space-y-2">
          <div className="flex items-center gap-1.5 xl:gap-1 2xl:gap-1.5">
            {isEditing ? (
              <div className="flex min-w-0 flex-1 items-center gap-1">
                <Input
                  value={editedName}
                  onChange={(e) => setEditedName(e.target.value)}
                  onKeyDown={handleKeyDown}
                  disabled={isSaving}
                  className="h-7 flex-1 text-sm"
                  onClick={(e) => e.stopPropagation()}
                />
                <span className="shrink-0 text-xs text-gray-600 dark:text-gray-400">
                  {getFileExtension(image.originalFilename)}
                </span>
                <Button
                  size="icon"
                  variant="ghost"
                  className="h-7 w-7 shrink-0 text-green-600 hover:bg-green-100 hover:text-green-700 dark:text-green-400 dark:hover:bg-green-950"
                  onClick={(e) => {
                    e.stopPropagation();
                    saveFilename();
                  }}
                  disabled={isSaving}
                >
                  <Check className="h-3.5 w-3.5" />
                </Button>
                <Button
                  size="icon"
                  variant="ghost"
                  className="h-7 w-7 shrink-0 text-red-600 hover:bg-red-100 hover:text-red-700 dark:text-red-400 dark:hover:bg-red-950"
                  onClick={(e) => {
                    e.stopPropagation();
                    cancelEditing();
                  }}
                  disabled={isSaving}
                >
                  <X className="h-3.5 w-3.5" />
                </Button>
              </div>
            ) : (
              <>
                <h3
                  className="min-w-0 flex-1 truncate text-sm leading-tight font-semibold text-gray-900 xl:text-xs 2xl:text-sm dark:text-white"
                  title={image.originalFilename}
                >
                  {image.originalFilename}
                </h3>
                <Button
                  size="icon"
                  variant="ghost"
                  className="h-6 w-6 shrink-0 text-gray-500 hover:text-gray-700 dark:text-gray-400 dark:hover:text-gray-200"
                  onClick={(e) => {
                    e.preventDefault();
                    e.stopPropagation();
                    startEditing();
                  }}
                >
                  <Pencil className="h-3.5 w-3.5" />
                </Button>
              </>
            )}
            <Badge variant="secondary" className="shrink-0 text-xs font-semibold">
              {mimeLabel}
            </Badge>
            <Badge variant="outline" className={`shrink-0 text-xs font-medium ${statusClass}`}>
              {statusLabel}
            </Badge>
          </div>

          <div className="bg-muted/30 grid grid-cols-2 gap-2 rounded-lg p-2.5 xl:gap-1.5 xl:p-2 2xl:gap-2 2xl:p-2.5">
            <div className="flex items-center gap-1.5">
              <div className="flex h-6 w-6 items-center justify-center rounded bg-cyan-100 dark:bg-cyan-950">
                <ImageIcon className="h-3.5 w-3.5 text-cyan-600 dark:text-cyan-400" />
              </div>
              <span className="text-xs font-medium text-gray-700 xl:text-[11px] 2xl:text-xs dark:text-gray-300">
                {image.width} × {image.height}
              </span>
            </div>

            <div className="flex items-center gap-1.5">
              <div className="flex h-6 w-6 items-center justify-center rounded bg-blue-100 dark:bg-blue-950">
                <Calendar className="h-3.5 w-3.5 text-blue-600 dark:text-blue-400" />
              </div>
              <span className="text-xs font-medium text-gray-700 xl:text-[11px] 2xl:text-xs dark:text-gray-300">
                {formatDate(image.createdAt, resolvedTimezone)}
              </span>
            </div>

            <div className="border-muted col-span-2 flex items-center gap-1.5 border-t pt-2 xl:pt-1.5 2xl:pt-2">
              <span className="text-muted-foreground text-xs font-semibold xl:text-[11px] 2xl:text-xs">Size:</span>
              <span className="text-xs font-bold text-gray-900 xl:text-[11px] 2xl:text-xs dark:text-white">
                {formatFileSize(image.sizeBytes)}
              </span>
            </div>
          </div>
        </div>

        <div className="flex gap-2 xl:gap-1.5 2xl:gap-2">
          <Button
            asChild
            variant="outline"
            size="sm"
            className="flex-1 gap-2 border-cyan-200 bg-cyan-50 text-cyan-700 hover:bg-cyan-100 hover:text-cyan-800 xl:text-xs 2xl:text-sm dark:border-cyan-800 dark:bg-cyan-950 dark:text-cyan-300 dark:hover:bg-cyan-900"
          >
            <Link href={`/preprocessing?imageId=${image.id}`}>
              <Eye className="h-4 w-4" />
              <span className="font-semibold">Process</span>
            </Link>
          </Button>
          {image.status === "archived" ? (
            <Button
              variant="outline"
              size="sm"
              onClick={(e) => {
                e.preventDefault();
                e.stopPropagation();
                onUnarchive(image.id);
              }}
              className="gap-2 border-amber-200 bg-amber-50 text-amber-700 hover:bg-amber-100 hover:text-amber-800 xl:text-xs 2xl:text-sm dark:border-amber-800 dark:bg-amber-950 dark:text-amber-300 dark:hover:bg-amber-900"
              title="Unarchive"
            >
              <ArchiveRestore className="h-4 w-4" />
            </Button>
          ) : (
            <Button
              variant="outline"
              size="sm"
              onClick={(e) => {
                e.preventDefault();
                e.stopPropagation();
                onArchive(image.id);
              }}
              className="gap-2 border-gray-200 bg-gray-50 text-gray-700 hover:bg-gray-100 hover:text-gray-800 xl:text-xs 2xl:text-sm dark:border-gray-700 dark:bg-gray-900 dark:text-gray-300 dark:hover:bg-gray-800"
              title="Archive"
            >
              <Archive className="h-4 w-4" />
            </Button>
          )}
          <Button
            variant="outline"
            size="sm"
            onClick={(e) => {
              e.preventDefault();
              e.stopPropagation();
              onDelete(image.id);
            }}
            className="gap-2 border-red-200 bg-red-50 text-red-700 hover:bg-red-100 hover:text-red-800 xl:text-xs 2xl:text-sm dark:border-red-800 dark:bg-red-950 dark:text-red-300 dark:hover:bg-red-900"
            title="Delete"
          >
            <Trash2 className="h-4 w-4" />
          </Button>
        </div>
      </CardContent>
    </Card>
  );
};
