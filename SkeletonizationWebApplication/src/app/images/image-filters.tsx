"use client";

import { Filter, Search, SlidersHorizontal, X } from "lucide-react";
import type { ReactNode } from "react";

import { Badge } from "@/components/ui/badge";
import { Button } from "@/components/ui/button";
import {
  DropdownMenu,
  DropdownMenuCheckboxItem,
  DropdownMenuContent,
  DropdownMenuLabel,
  DropdownMenuSeparator,
  DropdownMenuTrigger
} from "@/components/ui/dropdown-menu";
import { Input } from "@/components/ui/input";

import { type ImageFormat, type SizeFilter } from "./types";
import { getSizeLabel } from "./utils";

type ImageFiltersProps = {
  searchQuery: string;
  onSearchChange: (value: string) => void;
  selectedFormats: Set<ImageFormat>;
  selectedSizes: Set<SizeFilter>;
  activeFilterCount: number;
  toggleFormat: (format: ImageFormat) => void;
  toggleSize: (size: SizeFilter) => void;
  clearAllFilters: () => void;
  sortControl?: ReactNode;
};

export const ImageFilters = ({
  searchQuery,
  onSearchChange,
  selectedFormats,
  selectedSizes,
  activeFilterCount,
  toggleFormat,
  toggleSize,
  clearAllFilters,
  sortControl
}: ImageFiltersProps) => (
  <div className="space-y-2">
    <div className="flex flex-col gap-2 sm:flex-row sm:items-center">
      <div className="relative flex-1">
        <Search className="text-muted-foreground absolute top-1/2 left-3 h-4 w-4 -translate-y-1/2 xl:h-3.5 xl:w-3.5 2xl:h-4 2xl:w-4" />
        <Input
          type="text"
          placeholder="Search images..."
          value={searchQuery}
          onChange={(e) => onSearchChange(e.target.value)}
          className="h-9 pl-9 text-sm xl:h-8 xl:text-xs 2xl:h-9 2xl:text-sm"
        />
      </div>

      <div className="flex flex-wrap gap-2">
        {sortControl}

        <DropdownMenu>
          <DropdownMenuTrigger asChild>
            <Button variant="outline" size="sm" className="h-9 gap-1.5 xl:h-8 2xl:h-9">
              <Filter className="h-3.5 w-3.5 2xl:h-4 2xl:w-4" />
              <span className="hidden text-xs sm:inline 2xl:text-sm">Format</span>
              {selectedFormats.size > 0 && (
                <Badge variant="secondary" className="h-4 min-w-4 rounded-full px-1 text-[10px] 2xl:h-5 2xl:text-xs">
                  {selectedFormats.size}
                </Badge>
              )}
            </Button>
          </DropdownMenuTrigger>
          <DropdownMenuContent align="end" className="w-40">
            <DropdownMenuLabel className="text-xs">Format</DropdownMenuLabel>
            <DropdownMenuSeparator />
            <DropdownMenuCheckboxItem
              checked={selectedFormats.has("png")}
              onCheckedChange={() => toggleFormat("png")}
              className="text-xs"
            >
              PNG
            </DropdownMenuCheckboxItem>
            <DropdownMenuCheckboxItem
              checked={selectedFormats.has("jpeg")}
              onCheckedChange={() => toggleFormat("jpeg")}
              className="text-xs"
            >
              JPEG
            </DropdownMenuCheckboxItem>
            <DropdownMenuCheckboxItem
              checked={selectedFormats.has("bmp")}
              onCheckedChange={() => toggleFormat("bmp")}
              className="text-xs"
            >
              BMP
            </DropdownMenuCheckboxItem>
            <DropdownMenuCheckboxItem
              checked={selectedFormats.has("tiff")}
              onCheckedChange={() => toggleFormat("tiff")}
              className="text-xs"
            >
              TIFF
            </DropdownMenuCheckboxItem>
          </DropdownMenuContent>
        </DropdownMenu>

        <DropdownMenu>
          <DropdownMenuTrigger asChild>
            <Button variant="outline" size="sm" className="h-9 gap-1.5 xl:h-8 2xl:h-9">
              <SlidersHorizontal className="h-3.5 w-3.5 2xl:h-4 2xl:w-4" />
              <span className="hidden text-xs sm:inline 2xl:text-sm">Size</span>
              {selectedSizes.size > 0 && (
                <Badge variant="secondary" className="h-4 min-w-4 rounded-full px-1 text-[10px] 2xl:h-5 2xl:text-xs">
                  {selectedSizes.size}
                </Badge>
              )}
            </Button>
          </DropdownMenuTrigger>
          <DropdownMenuContent align="end" className="w-44">
            <DropdownMenuLabel className="text-xs">Resolution</DropdownMenuLabel>
            <DropdownMenuSeparator />
            <DropdownMenuCheckboxItem
              checked={selectedSizes.has("small")}
              onCheckedChange={() => toggleSize("small")}
              className="text-xs"
            >
              Small {getSizeLabel("small")}
            </DropdownMenuCheckboxItem>
            <DropdownMenuCheckboxItem
              checked={selectedSizes.has("medium")}
              onCheckedChange={() => toggleSize("medium")}
              className="text-xs"
            >
              Medium {getSizeLabel("medium")}
            </DropdownMenuCheckboxItem>
            <DropdownMenuCheckboxItem
              checked={selectedSizes.has("large")}
              onCheckedChange={() => toggleSize("large")}
              className="text-xs"
            >
              Large {getSizeLabel("large")}
            </DropdownMenuCheckboxItem>
            <DropdownMenuCheckboxItem
              checked={selectedSizes.has("xlarge")}
              onCheckedChange={() => toggleSize("xlarge")}
              className="text-xs"
            >
              X-Large {getSizeLabel("xlarge")}
            </DropdownMenuCheckboxItem>
          </DropdownMenuContent>
        </DropdownMenu>

        {activeFilterCount > 0 && (
          <Button
            variant="ghost"
            size="sm"
            onClick={clearAllFilters}
            className="h-9 gap-1.5 xl:h-8 2xl:h-9"
            title="Clear all filters"
          >
            <X className="h-3.5 w-3.5 2xl:h-4 2xl:w-4" />
            <span className="text-xs 2xl:text-sm">Clear</span>
          </Button>
        )}
      </div>
    </div>

    {(selectedFormats.size > 0 || selectedSizes.size > 0) && (
      <div className="flex flex-wrap items-center gap-1.5">
        {Array.from(selectedFormats).map((format) => (
          <Badge key={format} variant="secondary" className="h-6 gap-1 pr-0.5 pl-2 text-[11px]">
            {format.toUpperCase()}
            <button
              onClick={() => toggleFormat(format)}
              className="hover:bg-muted-foreground/20 ml-0.5 rounded-sm p-0.5 transition-colors"
              aria-label={`Remove ${format} filter`}
            >
              <X className="h-3 w-3" />
            </button>
          </Badge>
        ))}

        {Array.from(selectedSizes).map((size) => (
          <Badge key={size} variant="secondary" className="h-6 gap-1 pr-0.5 pl-2 text-[11px]">
            {getSizeLabel(size)}
            <button
              onClick={() => toggleSize(size)}
              className="hover:bg-muted-foreground/20 ml-0.5 rounded-sm p-0.5 transition-colors"
              aria-label={`Remove ${size} filter`}
            >
              <X className="h-3 w-3" />
            </button>
          </Badge>
        ))}
      </div>
    )}
  </div>
);
