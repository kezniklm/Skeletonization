"use client";

import { ArrowDownAZ, ArrowUpAZ, Calendar } from "lucide-react";

import { Button } from "@/components/ui/button";
import {
  DropdownMenu,
  DropdownMenuContent,
  DropdownMenuItem,
  DropdownMenuTrigger
} from "@/components/ui/dropdown-menu";

import { type SortOption } from "./types";

type ImageSortProps = {
  sortBy: SortOption;
  onSortChange: (sort: SortOption) => void;
};

export const ImageSort = ({ sortBy, onSortChange }: ImageSortProps) => {
  const sortOptions = [
    { value: "date-desc" as const, label: "Newest First", icon: Calendar },
    { value: "date-asc" as const, label: "Oldest First", icon: Calendar },
    { value: "name-asc" as const, label: "Name (A-Z)", icon: ArrowDownAZ },
    { value: "name-desc" as const, label: "Name (Z-A)", icon: ArrowUpAZ }
  ];

  const currentSort = sortOptions.find((opt) => opt.value === sortBy);

  return (
    <DropdownMenu>
      <DropdownMenuTrigger asChild>
        <Button variant="outline" className="gap-2">
          {currentSort?.icon && <currentSort.icon className="h-4 w-4" />}
          <span className="hidden sm:inline">Sort: {currentSort?.label}</span>
          <span className="sm:hidden">{currentSort?.label}</span>
        </Button>
      </DropdownMenuTrigger>
      <DropdownMenuContent align="start" className="w-48">
        {sortOptions.map((option) => (
          <DropdownMenuItem
            key={option.value}
            onClick={() => onSortChange(option.value)}
            className={sortBy === option.value ? "bg-accent" : ""}
          >
            <option.icon className="mr-2 h-4 w-4" />
            {option.label}
          </DropdownMenuItem>
        ))}
      </DropdownMenuContent>
    </DropdownMenu>
  );
};
