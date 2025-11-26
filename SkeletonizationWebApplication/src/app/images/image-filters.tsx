"use client";

import { Filter, Search, SlidersHorizontal, X } from "lucide-react";
import { type ReactNode } from "react";

import { Badge } from "@/components/ui/badge";
import { Button } from "@/components/ui/button";
import { Card, CardContent, CardDescription, CardHeader, CardTitle } from "@/components/ui/card";
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
  <Card className="border-gray-200 bg-white shadow-sm backdrop-blur-sm dark:border-gray-800 dark:bg-gray-900/95">
    <CardHeader className="border-b border-gray-200 bg-linear-to-r from-cyan-50/50 to-blue-50/50 pb-3 dark:border-gray-800 dark:from-cyan-950/20 dark:to-blue-950/20">
      <div className="space-y-0.5">
        <CardTitle className="text-lg text-gray-900 dark:text-white">Filter & Search</CardTitle>
        <CardDescription className="text-sm text-gray-600 dark:text-gray-400">
          Find and organize your images
        </CardDescription>
      </div>
    </CardHeader>

    <CardContent className="space-y-4 pt-4">
      <div className="flex flex-col gap-3 sm:flex-row sm:items-center">
        <div className="relative flex-1">
          <Search className="text-muted-foreground absolute top-1/2 left-3 h-4 w-4 -translate-y-1/2" />
          <Input
            type="text"
            placeholder="Search by filename..."
            value={searchQuery}
            onChange={(e) => onSearchChange(e.target.value)}
            className="pl-9"
          />
        </div>

        <div className="flex flex-wrap gap-2">
          {sortControl}

          <DropdownMenu>
            <DropdownMenuTrigger asChild>
              <Button variant="outline" size="default" className="gap-2">
                <Filter className="h-4 w-4" />
                Format
                {selectedFormats.size > 0 && (
                  <Badge variant="secondary" className="ml-1 h-5 rounded-full px-1.5 text-xs">
                    {selectedFormats.size}
                  </Badge>
                )}
              </Button>
            </DropdownMenuTrigger>
            <DropdownMenuContent align="end" className="w-52">
              <DropdownMenuLabel>Filter by Format</DropdownMenuLabel>
              <DropdownMenuSeparator />
              <DropdownMenuCheckboxItem
                checked={selectedFormats.has("png")}
                onCheckedChange={() => toggleFormat("png")}
              >
                PNG
              </DropdownMenuCheckboxItem>
              <DropdownMenuCheckboxItem
                checked={selectedFormats.has("jpeg")}
                onCheckedChange={() => toggleFormat("jpeg")}
              >
                JPEG
              </DropdownMenuCheckboxItem>
              <DropdownMenuCheckboxItem
                checked={selectedFormats.has("bmp")}
                onCheckedChange={() => toggleFormat("bmp")}
              >
                BMP
              </DropdownMenuCheckboxItem>
              <DropdownMenuCheckboxItem
                checked={selectedFormats.has("tiff")}
                onCheckedChange={() => toggleFormat("tiff")}
              >
                TIFF
              </DropdownMenuCheckboxItem>
            </DropdownMenuContent>
          </DropdownMenu>

          <DropdownMenu>
            <DropdownMenuTrigger asChild>
              <Button variant="outline" size="default" className="gap-2">
                <SlidersHorizontal className="h-4 w-4" />
                Size
                {selectedSizes.size > 0 && (
                  <Badge variant="secondary" className="ml-1 h-5 rounded-full px-1.5 text-xs">
                    {selectedSizes.size}
                  </Badge>
                )}
              </Button>
            </DropdownMenuTrigger>
            <DropdownMenuContent align="end" className="w-52">
              <DropdownMenuLabel>Filter by Resolution</DropdownMenuLabel>
              <DropdownMenuSeparator />
              <DropdownMenuCheckboxItem
                checked={selectedSizes.has("small")}
                onCheckedChange={() => toggleSize("small")}
              >
                Small {getSizeLabel("small")}
              </DropdownMenuCheckboxItem>
              <DropdownMenuCheckboxItem
                checked={selectedSizes.has("medium")}
                onCheckedChange={() => toggleSize("medium")}
              >
                Medium {getSizeLabel("medium")}
              </DropdownMenuCheckboxItem>
              <DropdownMenuCheckboxItem
                checked={selectedSizes.has("large")}
                onCheckedChange={() => toggleSize("large")}
              >
                Large {getSizeLabel("large")}
              </DropdownMenuCheckboxItem>
              <DropdownMenuCheckboxItem
                checked={selectedSizes.has("xlarge")}
                onCheckedChange={() => toggleSize("xlarge")}
              >
                X-Large {getSizeLabel("xlarge")}
              </DropdownMenuCheckboxItem>
            </DropdownMenuContent>
          </DropdownMenu>

          {activeFilterCount > 0 && (
            <Button variant="ghost" size="icon" onClick={clearAllFilters} title="Clear all filters">
              <X className="h-4 w-4" />
            </Button>
          )}
        </div>
      </div>

      {(selectedFormats.size > 0 || selectedSizes.size > 0) && (
        <div className="flex flex-wrap items-center gap-2">
          <span className="text-sm font-semibold text-gray-700 dark:text-gray-300">Active filters:</span>

          {Array.from(selectedFormats).map((format) => (
            <Badge key={format} variant="secondary" className="gap-1.5 pr-1 pl-2">
              <span className="text-xs font-medium">{format.toUpperCase()}</span>
              <button
                onClick={() => toggleFormat(format)}
                className="hover:bg-muted-foreground/20 rounded-sm transition-colors"
              >
                <X className="h-3 w-3" />
              </button>
            </Badge>
          ))}

          {Array.from(selectedSizes).map((size) => (
            <Badge key={size} variant="secondary" className="gap-1.5 pr-1 pl-2">
              <span className="text-xs font-medium">{getSizeLabel(size)}</span>
              <button
                onClick={() => toggleSize(size)}
                className="hover:bg-muted-foreground/20 rounded-sm transition-colors"
              >
                <X className="h-3 w-3" />
              </button>
            </Badge>
          ))}
        </div>
      )}
    </CardContent>
  </Card>
);
