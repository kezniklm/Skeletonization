"use client";

import type { ReactNode } from "react";

import { ClearFiltersButton, FilterBar } from "@/components/filters";

type LabFiltersProps = {
  searchQuery: string;
  onSearchChange: (value: string) => void;
  activeFilterCount: number;
  clearAllFilters: () => void;
  sortControl?: ReactNode;
};

export const LabFilters = ({
  searchQuery,
  onSearchChange,
  activeFilterCount,
  clearAllFilters,
  sortControl
}: LabFiltersProps) => (
  <FilterBar searchValue={searchQuery} onSearchChange={onSearchChange} searchPlaceholder="Search runs...">
    {sortControl}
    <ClearFiltersButton onClick={clearAllFilters} show={activeFilterCount > 0} />
  </FilterBar>
);
