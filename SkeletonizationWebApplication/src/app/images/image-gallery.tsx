"use client";

import type { SelectImage } from "@/database/zod/image";
import { Badge } from "@/components/ui/badge";
import { Tabs, TabsContent, TabsList, TabsTrigger } from "@/components/ui/tabs";
import { useImageGallery } from "@/hooks/use-image-gallery";
import { type FilterType } from "@/components/filters";

import { ImageFilters } from "./image-filters";
import { ImageGrid } from "./image-grid";
import { ImageDeleteDialog } from "./image-delete-dialog";
import { ImageSort } from "./image-sort";
import { ImagePagination } from "./image-pagination";

const ImageGallery = ({ initialImages }: { initialImages: SelectImage[] }) => {
  const {
    images,
    paginatedImages,
    filterOptions,
    selectedFilter,
    searchQuery,
    selectedFormats,
    selectedSizes,
    deleteDialogOpen,
    sortBy,
    page,
    totalPages,

    activeFilterCount,

    setSelectedFilter,
    setSearchQuery,
    setDeleteDialogOpen,
    setSortBy,
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
  } = useImageGallery(initialImages);

  const hasAdvancedFilters = selectedFormats.size > 0 || selectedSizes.size > 0;

  return (
    <div className="space-y-4">
      <ImageFilters
        searchQuery={searchQuery}
        onSearchChange={setSearchQuery}
        selectedFormats={selectedFormats}
        selectedSizes={selectedSizes}
        activeFilterCount={activeFilterCount}
        toggleFormat={toggleFormat}
        toggleSize={toggleSize}
        clearAllFilters={clearAllFilters}
        sortControl={<ImageSort sortBy={sortBy} onSortChange={setSortBy} />}
      />

      <div className="flex flex-col gap-2 sm:flex-row sm:items-center sm:justify-between">
        <Tabs
          value={selectedFilter}
          onValueChange={(value) => {
            setSelectedFilter(value as FilterType);
            setPage(1);
          }}
          className="flex-1"
        >
          <TabsList className="grid h-9 w-full grid-cols-5 xl:h-8 2xl:h-9">
            {filterOptions.map((option) => (
              <TabsTrigger key={option.value} value={option.value} className="gap-1.5 text-xs 2xl:text-sm">
                <span className="hidden sm:inline">{option.label}</span>
                <span className="sm:hidden">{option.label.split(" ")[0]}</span>
                <Badge
                  variant="secondary"
                  className="h-4 min-w-4 rounded-full px-1.5 text-[10px] font-medium 2xl:h-5 2xl:px-2 2xl:text-xs"
                >
                  {option.count}
                </Badge>
              </TabsTrigger>
            ))}
          </TabsList>
        </Tabs>

        {totalPages > 1 && (
          <div className="flex justify-center sm:justify-end">
            <ImagePagination currentPage={page} totalPages={totalPages} onPageChange={setPage} />
          </div>
        )}
      </div>

      <Tabs value={selectedFilter}>
        {filterOptions.map((option) => (
          <TabsContent key={option.value} value={option.value} className="mt-0">
            <ImageGrid
              images={images}
              filteredImages={paginatedImages}
              searchQuery={searchQuery}
              hasAdvancedFilters={hasAdvancedFilters}
              selectedFilter={selectedFilter}
              onUploadComplete={handleUploadComplete}
              onDelete={openDeleteDialog}
              onRename={handleRename}
              onArchive={handleArchive}
              onUnarchive={handleUnarchive}
            />
          </TabsContent>
        ))}
      </Tabs>

      {totalPages > 1 && (
        <div className="flex justify-center">
          <ImagePagination currentPage={page} totalPages={totalPages} onPageChange={setPage} />
        </div>
      )}

      <ImageDeleteDialog open={deleteDialogOpen} onOpenChange={setDeleteDialogOpen} onConfirm={handleDelete} />
    </div>
  );
};

export default ImageGallery;
