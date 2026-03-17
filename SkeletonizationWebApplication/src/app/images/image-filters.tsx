/**
 * @file image-filters.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Implements filter controls for image gallery view.
 * @description Provides search integration, format/size multi-select filters, and filter reset controls.
 * @version 1.0
 * @date 2026-03-16
 */

"use client";

import { Filter, SlidersHorizontal } from "lucide-react";
import type { ReactNode } from "react";

import {
  ClearFiltersButton,
  FilterBar,
  FilterDropdown,
  type FilterOption,
  type ImageFormat,
  type SizeFilter
} from "@/components/filters";
import { DEFAULT_OUTPUT_FORMAT_ENUM } from "@/database/schema";

import { getSizeLabel } from "./utils";

/**
 * @brief Represents image filter component input properties.
 */
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

/**
 * @brief Declares selectable image format filter options.
 * @description Generated from default output format enum values.
 */
const formatOptions = DEFAULT_OUTPUT_FORMAT_ENUM.map((format) => ({
  value: format.toLowerCase() as ImageFormat,
  label: format
})) satisfies FilterOption<ImageFormat>[];

/**
 * @brief Declares selectable image size filter options.
 * @description Uses utility size labels for user-readable megapixel ranges.
 */
const sizeOptions: FilterOption<SizeFilter>[] = [
  { value: "small", label: `Small ${getSizeLabel("small")}` },
  { value: "medium", label: `Medium ${getSizeLabel("medium")}` },
  { value: "large", label: `Large ${getSizeLabel("large")}` },
  { value: "xlarge", label: `X-Large ${getSizeLabel("xlarge")}` }
];

/**
 * @brief Renders image search and advanced filter controls.
 * @param searchQuery Current search query.
 * @param onSearchChange Callback for search query updates.
 * @param selectedFormats Selected output formats.
 * @param selectedSizes Selected size buckets.
 * @param activeFilterCount Number of active filters.
 * @param toggleFormat Callback to toggle format filter.
 * @param toggleSize Callback to toggle size filter.
 * @param clearAllFilters Callback to reset all filters.
 * @param sortControl Optional sort control element.
 * @returns Filter controls JSX.
 */
/** @brief Renders search and dropdown filters for image gallery. */
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
}: ImageFiltersProps) => {
  /**
   * @brief Builds summary label for selected format filters.
   * @param selected Currently selected format values.
   * @returns Compact summary text for filter trigger.
   */
  const getFormatSummary = (selected: Set<ImageFormat>) => {
    if (selected.size === 0) return "Format";
    let first: string | undefined;

    for (const format of DEFAULT_OUTPUT_FORMAT_ENUM) {
      if (selected.has(format.toLowerCase() as ImageFormat)) {
        first = format;
        break;
      }
    }

    if (selected.size === 1) return `Format: ${first}`;
    return `Format: ${first} +${selected.size - 1}`;
  };

  /**
   * @brief Builds summary label for selected size filters.
   * @param selected Currently selected size values.
   * @returns Compact summary text for filter trigger.
   */
  const getSizeSummary = (selected: Set<SizeFilter>) => {
    if (selected.size === 0) return "Size";
    const firstValue = selected.values().next().value;
    if (!firstValue) return "Size";
    const first = getSizeLabel(firstValue);
    if (selected.size === 1) return `Size: ${first}`;
    return `Size: ${first} +${selected.size - 1}`;
  };

  return (
    <FilterBar searchValue={searchQuery} onSearchChange={onSearchChange} searchPlaceholder="Search images...">
      {sortControl}

      <FilterDropdown
        icon={Filter}
        label="Format"
        selectedValues={selectedFormats}
        options={formatOptions}
        onToggle={toggleFormat}
        getSummary={getFormatSummary}
      />

      <FilterDropdown
        icon={SlidersHorizontal}
        label="Size"
        selectedValues={selectedSizes}
        options={sizeOptions}
        onToggle={toggleSize}
        getSummary={getSizeSummary}
        menuWidth="w-44"
      />

      <ClearFiltersButton onClick={clearAllFilters} show={activeFilterCount > 0} />
    </FilterBar>
  );
};
