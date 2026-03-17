/**
 * @file lab-filters.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Provides search and clear filter controls for lab history.
 * @description Wraps shared filter bar and clear action for run filtering workflows.
 * @version 1.0
 * @date 2026-03-16
 */

"use client";

import type { ReactNode } from "react";

import { ClearFiltersButton, FilterBar } from "@/components/filters";

/**
 * @brief Represents lab filter bar properties.
 */
type LabFiltersProps = {
  searchQuery: string;
  onSearchChange: (value: string) => void;
  activeFilterCount: number;
  clearAllFilters: () => void;
  sortControl?: ReactNode;
};

/**
 * @brief Renders lab history search/filter bar.
 * @param searchQuery Current query string.
 * @param onSearchChange Callback to update search query.
 * @param activeFilterCount Number of active filters.
 * @param clearAllFilters Callback to reset filters.
 * @param sortControl Optional sort/filter controls.
 * @returns Filter bar JSX.
 */
/** @brief Renders lab search bar with optional sort/filter controls. */
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
