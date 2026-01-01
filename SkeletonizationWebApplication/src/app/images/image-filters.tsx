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

const formatOptions = DEFAULT_OUTPUT_FORMAT_ENUM.map((format) => ({
  value: format.toLowerCase() as ImageFormat,
  label: format
})) satisfies FilterOption<ImageFormat>[];

const sizeOptions: FilterOption<SizeFilter>[] = [
  { value: "small", label: `Small ${getSizeLabel("small")}` },
  { value: "medium", label: `Medium ${getSizeLabel("medium")}` },
  { value: "large", label: `Large ${getSizeLabel("large")}` },
  { value: "xlarge", label: `X-Large ${getSizeLabel("xlarge")}` }
];

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
