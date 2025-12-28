"use client";

import { Search, X } from "lucide-react";
import type { ReactNode } from "react";

import { Button } from "@/components/ui/button";
import { Input } from "@/components/ui/input";

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
  <div className="flex flex-col gap-2 sm:flex-row sm:items-center">
    <div className="relative flex-1">
      <Search className="text-muted-foreground absolute top-1/2 left-3 h-4 w-4 -translate-y-1/2 xl:h-3.5 xl:w-3.5 2xl:h-4 2xl:w-4" />
      <Input
        type="text"
        placeholder="Search runs..."
        value={searchQuery}
        onChange={(e) => onSearchChange(e.target.value)}
        className="h-9 pl-9 text-sm xl:h-8 xl:text-xs 2xl:h-9 2xl:text-sm"
      />
    </div>

    <div className="flex flex-wrap gap-2">
      {sortControl}

      {activeFilterCount > 0 && (
        <Button
          variant="ghost"
          size="sm"
          onClick={clearAllFilters}
          className="h-9 gap-1.5 xl:h-8 2xl:h-9"
          title="Clear filters"
        >
          <X className="h-3.5 w-3.5 2xl:h-4 2xl:w-4" />
          <span className="text-xs 2xl:text-sm">Clear</span>
        </Button>
      )}
    </div>
  </div>
);
