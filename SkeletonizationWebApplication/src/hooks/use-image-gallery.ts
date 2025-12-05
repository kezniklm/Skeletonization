"use client";

import { useState } from "react";
import { toast } from "sonner";

import { type FilterType, type ImageFormat, type SizeFilter, type SortOption } from "@/app/images/types";
import type { SelectImage } from "@/database/zod/image";
import { archiveImageAction, deleteImageAction, unarchiveImageAction } from "@/server-actions/images";

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

  const filterOptions = [
    { value: "all" as const, label: "All Images", count: images.filter((img) => img.status !== "archived").length },
    {
      value: "uploaded" as const,
      label: "Uploaded",
      count: images.filter((img) => img.status === "uploaded").length
    },
    {
      value: "validated" as const,
      label: "Validated",
      count: images.filter((img) => img.status === "validated").length
    },
    {
      value: "archived" as const,
      label: "Archived",
      count: images.filter((img) => img.status === "archived").length
    },
    {
      value: "derived" as const,
      label: "Derived",
      count: images.filter((img) => img.status === "derived").length
    }
  ];

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

    try {
      await deleteImageAction(imageToDelete);
      setImages((prev) => prev.filter((img) => img.id !== imageToDelete));
      toast.success("Image deleted");
      setDeleteDialogOpen(false);
      setImageToDelete(null);
    } catch (error) {
      console.error("Delete error:", error);
      toast.error("Failed to delete image");
    }
  };

  const handleArchive = async (imageId: string) => {
    try {
      await archiveImageAction(imageId);
      setImages((prev) => prev.map((img) => (img.id === imageId ? { ...img, status: "archived" } : img)));
      toast.success("Image archived");
    } catch (error) {
      console.error("Archive error:", error);
      toast.error("Failed to archive image");
    }
  };

  const handleUnarchive = async (imageId: string) => {
    try {
      await unarchiveImageAction(imageId);
      setImages((prev) => prev.map((img) => (img.id === imageId ? { ...img, status: "uploaded" } : img)));
      toast.success("Image unarchived");
    } catch (error) {
      console.error("Unarchive error:", error);
      toast.error("Failed to unarchive image");
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
