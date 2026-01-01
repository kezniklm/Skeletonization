"use client";

import { SortDropdown, type SortOption } from "@/components/filters";

export type LabSortOption = SortOption;

type LabSortProps = {
  sortBy: LabSortOption;
  onSortChange: (sort: LabSortOption) => void;
};

export const LabSort = ({ sortBy, onSortChange }: LabSortProps) => (
  <SortDropdown value={sortBy} onChange={onSortChange} />
);
