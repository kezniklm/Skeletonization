/**
 * @file image-sort.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Wraps shared sort dropdown for image gallery.
 * @description Provides typed adapter between gallery sorting state and reusable sort control component.
 * @version 1.0
 * @date 2026-03-16
 */

"use client";

import { SortDropdown, type SortOption } from "@/components/filters";

/**
 * @brief Represents image sort component properties.
 */
type ImageSortProps = {
  sortBy: SortOption;
  onSortChange: (sort: SortOption) => void;
};

/**
 * @brief Renders sort dropdown for image gallery ordering.
 * @param sortBy Current sort option.
 * @param onSortChange Callback for sort option updates.
 * @returns Sort dropdown JSX.
 */
export const ImageSort = ({ sortBy, onSortChange }: ImageSortProps) => (
  <SortDropdown value={sortBy} onChange={onSortChange} />
);
