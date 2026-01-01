"use client";

import type { ReactNode } from "react";

import { SearchInput } from "./search-input";

type FilterBarProps = {
  searchValue: string;
  onSearchChange: (value: string) => void;
  searchPlaceholder?: string;
  children?: ReactNode;
};

export const FilterBar = ({ searchValue, onSearchChange, searchPlaceholder, children }: FilterBarProps) => (
  <div className="flex flex-col gap-2 sm:flex-row sm:items-center">
    <SearchInput value={searchValue} onChange={onSearchChange} placeholder={searchPlaceholder} />
    <div className="flex flex-wrap gap-2">{children}</div>
  </div>
);
