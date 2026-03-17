/**
 * @file use-image-gallery.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Manages image gallery filtering and actions.
 * @description Provides state and handlers for filtering, searching, sorting, pagination, and image mutation operations.
 * @version 1.0
 * @date 2026-03-16
 */

"use client";

import { useState } from "react";
import { toast } from "sonner";

import { type FilterType, type ImageFormat, type SizeFilter, type SortOption } from "@/components/filters";
import type { SelectImage } from "@/database/zod/image";
import { archiveImageAction, deleteImageAction, unarchiveImageAction } from "@/server-actions/images";

type FilterOption = Readonly<{
  value: FilterType;
  label: string;
  count: number;
}>;

/**
 * @brief Builds state and handlers for image gallery views.
 * @description Manages image collection transforms and server action wrappers for delete, archive, and unarchive operations.
 * @param initialImages Initial image dataset.
 * @returns Gallery state, derived values, and action handlers.
 */
export const useImageGallery = (initialImages: SelectImage[]) => {
  const [images, setImages] = useState<SelectImage[]>(initialImages);
  const [selectedFilter, setSelectedFilter] = useState<FilterType>("all");
  const [searchQuery, setSearchQueryState] = useState("");
  const [deleteDialogOpen, setDeleteDialogOpen] = useState(false);
  const [imageToDelete, setImageToDelete] = useState<string | null>(null);
  const [sortBy, setSortByState] = useState<SortOption>("date-desc");
  const [page, setPage] = useState(1);
  const [pageSize] = useState(20);

  const [selectedFormats, setSelectedFormats] = useState<Set<ImageFormat>>(new Set());
  const [selectedSizes, setSelectedSizes] = useState<Set<SizeFilter>>(new Set());

  const toggleFormat = (format: ImageFormat) => {
    setSelectedFormats((prev) => {
      const next = new Set(prev);
      if (next.has(format)) next.delete(format);
      else next.add(format);
      return next;
    });
    setPage(1);
  };

  const toggleSize = (size: SizeFilter) => {
    setSelectedSizes((prev) => {
      const next = new Set(prev);
      if (next.has(size)) next.delete(size);
      else next.add(size);
      return next;
    });
    setPage(1);
  };

  const clearAllFilters = () => {
    setSelectedFormats(new Set());
    setSelectedSizes(new Set());
    setSearchQueryState("");
    setPage(1);
  };

  const handleSearchChange = (query: string) => {
    setSearchQueryState(query);
    setPage(1);
  };

  const handleSortChange = (sort: SortOption) => {
    setSortByState(sort);
    setPage(1);
  };

  const activeFilterCount = (() => {
    let count = 0;
    if (selectedFormats.size > 0) count++;
    if (selectedSizes.size > 0) count++;
    return count;
  })();

  const filteredImages = (() => {
    let filtered = [...images];

    if (selectedFilter === "all") {
      filtered = filtered.filter((img) => img.status !== "archived");
    } else {
      filtered = filtered.filter((img) => img.status === selectedFilter);
    }

    if (selectedFormats.size > 0) {
      filtered = filtered.filter((img) => {
        const format = img.mime.split("/")[1]?.toLowerCase() as ImageFormat | undefined;
        return format ? selectedFormats.has(format) : false;
      });
    }

    if (selectedSizes.size > 0) {
      filtered = filtered.filter((img) => {
        const pixels = img.width * img.height;
        const size: SizeFilter =
          pixels < 1_000_000 ? "small" : pixels < 4_000_000 ? "medium" : pixels < 10_000_000 ? "large" : "xlarge";
        return selectedSizes.has(size);
      });
    }

    if (searchQuery.trim()) {
      const q = searchQuery.toLowerCase();
      filtered = filtered.filter((img) => img.originalFilename.toLowerCase().includes(q));
    }

    filtered.sort((a, b) => {
      switch (sortBy) {
        case "date-desc":
          return new Date(b.createdAt).getTime() - new Date(a.createdAt).getTime();
        case "date-asc":
          return new Date(a.createdAt).getTime() - new Date(b.createdAt).getTime();
        case "name-asc":
          return a.originalFilename.localeCompare(b.originalFilename);
        case "name-desc":
          return b.originalFilename.localeCompare(a.originalFilename);
        default:
          return 0;
      }
    });

    return filtered;
  })();

  const totalPages = Math.ceil(filteredImages.length / pageSize);

  const paginatedImages = (() => {
    const startIndex = (page - 1) * pageSize;
    const endIndex = startIndex + pageSize;
    return filteredImages.slice(startIndex, endIndex);
  })();

  const statusCounts = (() => {
    const counts: Record<Exclude<FilterType, "all">, number> = {
      uploaded: 0,
      skeletonized: 0,
      archived: 0,
      derived: 0
    };

    for (const img of images) {
      switch (img.status) {
        case "uploaded":
        case "skeletonized":
        case "archived":
        case "derived":
          counts[img.status] += 1;
          break;
        default:
          break;
      }
    }

    return counts;
  })();

  const filterOptions = [
    { value: "all", label: "All Images", count: images.length - statusCounts.archived },
    {
      value: "uploaded",
      label: "Uploaded",
      count: statusCounts.uploaded
    },
    {
      value: "skeletonized",
      label: "Skeletonized",
      count: statusCounts.skeletonized
    },
    {
      value: "archived",
      label: "Archived",
      count: statusCounts.archived
    },
    {
      value: "derived",
      label: "Derived",
      count: statusCounts.derived
    }
  ] satisfies readonly FilterOption[];

  const getImageLabel = (imageId: string) => images.find((img) => img.id === imageId)?.originalFilename ?? "image";

  const handleUploadComplete = (newImage: SelectImage) => {
    setImages((prev) => [newImage, ...prev]);
  };

  const handleRename = (imageId: string, newFilename: string) => {
    setImages((prev) => prev.map((img) => (img.id === imageId ? { ...img, originalFilename: newFilename } : img)));
  };

  const openDeleteDialog = (imageId: string) => {
    setImageToDelete(imageId);
    setDeleteDialogOpen(true);
  };

  const handleDelete = async () => {
    if (!imageToDelete) return;

    const imageLabel = getImageLabel(imageToDelete);

    try {
      await deleteImageAction(imageToDelete);
      setImages((prev) => prev.filter((img) => img.id !== imageToDelete));
      toast.success(`Image ${imageLabel} has been deleted successfully!`);
      setDeleteDialogOpen(false);
      setImageToDelete(null);
    } catch (error) {
      console.error("Delete error:", error);
      toast.error(`Failed to delete ${imageLabel}`);
    }
  };

  const handleArchive = async (imageId: string) => {
    const imageLabel = getImageLabel(imageId);
    try {
      await archiveImageAction(imageId);
      setImages((prev) => prev.map((img) => (img.id === imageId ? { ...img, status: "archived" } : img)));
      toast.success(`Image ${imageLabel} has been archived successfully!`);
    } catch (error) {
      console.error("Archive error:", error);
      toast.error(`Failed to archive ${imageLabel}`);
    }
  };

  const handleUnarchive = async (imageId: string) => {
    const imageLabel = getImageLabel(imageId);
    try {
      await unarchiveImageAction(imageId);
      setImages((prev) => prev.map((img) => (img.id === imageId ? { ...img, status: "uploaded" } : img)));
      toast.success(`Image ${imageLabel} has been unarchived successfully!`);
    } catch (error) {
      console.error("Unarchive error:", error);
      toast.error(`Failed to unarchive ${imageLabel}`);
    }
  };

  return {
    // data
    images,
    filteredImages,
    paginatedImages,
    filterOptions,
    selectedFilter,
    searchQuery,
    selectedFormats,
    selectedSizes,
    deleteDialogOpen,
    sortBy,
    page,
    pageSize,
    totalPages,

    // derived
    activeFilterCount,

    // setters / actions
    setSelectedFilter,
    setSearchQuery: handleSearchChange,
    setDeleteDialogOpen,
    setSortBy: handleSortChange,
    setPage,
    toggleFormat,
    toggleSize,
    clearAllFilters,
    handleUploadComplete,
    handleRename,
    openDeleteDialog,
    handleDelete,
    handleArchive,
    handleUnarchive
  };
};
