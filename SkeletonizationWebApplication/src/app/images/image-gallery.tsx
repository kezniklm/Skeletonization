"use client";

import type { SelectImage } from "@/database/zod/image";
import { Badge } from "@/components/ui/badge";
import { Tabs, TabsContent, TabsList, TabsTrigger } from "@/components/ui/tabs";
import { useImageGallery } from "@/hooks/use-image-gallery";

import { ImageFilters } from "./image-filters";
import { ImageGrid } from "./image-grid";
import { ImageDeleteDialog } from "./image-delete-dialog";
import { type FilterType } from "./types";
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
    handleDelete
  } = useImageGallery(initialImages);

  const hasAdvancedFilters = selectedFormats.size > 0 || selectedSizes.size > 0;

  return (
    <>
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

      <Tabs
        value={selectedFilter}
        onValueChange={(value) => {
          setSelectedFilter(value as FilterType);
          setPage(1);
        }}
      >
        <TabsList className="grid w-full grid-cols-5">
          {filterOptions.map((option) => (
            <TabsTrigger key={option.value} value={option.value} className="gap-2">
              <span className="hidden sm:inline">{option.label}</span>
              <span className="sm:hidden">{option.label.split(" ")[0]}</span>
              <Badge variant="outline" className="h-5 min-w-5 rounded-full px-1.5 text-xs">
                {option.count}
              </Badge>
            </TabsTrigger>
          ))}
        </TabsList>

        {filterOptions.map((option) => (
          <TabsContent key={option.value} value={option.value}>
            <ImageGrid
              images={images}
              filteredImages={paginatedImages}
              searchQuery={searchQuery}
              hasAdvancedFilters={hasAdvancedFilters}
              onUploadComplete={handleUploadComplete}
              onDelete={openDeleteDialog}
              onRename={handleRename}
            />
          </TabsContent>
        ))}
      </Tabs>

      {totalPages > 1 && <ImagePagination currentPage={page} totalPages={totalPages} onPageChange={setPage} />}

      <ImageDeleteDialog open={deleteDialogOpen} onOpenChange={setDeleteDialogOpen} onConfirm={handleDelete} />
    </>
  );
};

export default ImageGallery;
