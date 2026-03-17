/**
 * @file filter-bar.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Composes search and filter control layout.
 * @description Arranges a search input with optional auxiliary filter controls in a responsive bar.
 * @version 1.0
 * @date 2026-03-16
 */

"use client";

import type { ReactNode } from "react";

import { SearchInput } from "./search-input";

type FilterBarProps = {
  searchValue: string;
  onSearchChange: (value: string) => void;
  searchPlaceholder?: string;
  children?: ReactNode;
};

/**
 * @brief Renders a responsive filter bar container.
 * @description Provides a shared layout for search input and additional filter controls.
 * @param searchValue Current search query.
 * @param onSearchChange Callback for search query updates.
 * @param searchPlaceholder Optional placeholder text for search.
 * @param children Additional filter controls rendered next to search.
 * @returns A filter bar layout element.
 */
export const FilterBar = ({ searchValue, onSearchChange, searchPlaceholder, children }: FilterBarProps) => (
  <div className="flex flex-col gap-2 sm:flex-row sm:items-center">
    <SearchInput value={searchValue} onChange={onSearchChange} placeholder={searchPlaceholder} />
    <div className="flex flex-wrap gap-2">{children}</div>
  </div>
);
