"use client";

import { SortDropdown, type SortOption } from "@/components/filters";

type ImageSortProps = {
  sortBy: SortOption;
  onSortChange: (sort: SortOption) => void;
};

export const ImageSort = ({ sortBy, onSortChange }: ImageSortProps) => (
  <SortDropdown value={sortBy} onChange={onSortChange} />
);
