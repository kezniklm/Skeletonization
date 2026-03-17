/**
 * @file lab-sort.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Wraps shared sort dropdown for lab history ordering.
 * @description Defines typed sort alias and adapter component used by lab filter controls.
 * @version 1.0
 * @date 2026-03-16
 */

"use client";

import { SortDropdown, type SortOption } from "@/components/filters";

/**
 * @brief Alias for supported lab sort options.
 */
export type LabSortOption = SortOption;

/**
 * @brief Represents lab sort component properties.
 */
type LabSortProps = {
  sortBy: LabSortOption;
  onSortChange: (sort: LabSortOption) => void;
};

/**
 * @brief Renders lab history sort dropdown.
 * @param sortBy Selected sort option.
 * @param onSortChange Callback for sort option changes.
 * @returns Sort dropdown JSX.
 */
export const LabSort = ({ sortBy, onSortChange }: LabSortProps) => (
  <SortDropdown value={sortBy} onChange={onSortChange} />
);
