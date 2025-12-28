"use client";

import { ArrowDownAZ, ArrowUpAZ, ArrowUpDown, Calendar, type LucideIcon } from "lucide-react";

import { Button } from "@/components/ui/button";
import {
  DropdownMenu,
  DropdownMenuContent,
  DropdownMenuItem,
  DropdownMenuTrigger
} from "@/components/ui/dropdown-menu";

export type LabSortOption = "date-desc" | "date-asc" | "name-asc" | "name-desc";

type SortOption = {
  value: LabSortOption;
  label: string;
  icon: LucideIcon;
};

const sortOptions: SortOption[] = [
  { value: "date-desc", label: "Newest First", icon: Calendar },
  { value: "date-asc", label: "Oldest First", icon: Calendar },
  { value: "name-asc", label: "Name (A-Z)", icon: ArrowDownAZ },
  { value: "name-desc", label: "Name (Z-A)", icon: ArrowUpAZ }
];

type LabSortProps = {
  sortBy: LabSortOption;
  onSortChange: (sort: LabSortOption) => void;
};

export const LabSort = ({ sortBy, onSortChange }: LabSortProps) => (
  <DropdownMenu>
    <DropdownMenuTrigger asChild>
      <Button variant="outline" size="sm" className="h-9 gap-1.5 xl:h-8 2xl:h-9">
        <ArrowUpDown className="h-3.5 w-3.5 2xl:h-4 2xl:w-4" />
        <span className="text-xs 2xl:text-sm">{sortOptions.find((o) => o.value === sortBy)?.label ?? "Sort"}</span>
      </Button>
    </DropdownMenuTrigger>
    <DropdownMenuContent align="end" className="w-44">
      {sortOptions.map((option) => (
        <DropdownMenuItem
          key={option.value}
          onClick={() => onSortChange(option.value)}
          className={`text-xs ${sortBy === option.value ? "bg-accent" : ""}`}
        >
          <option.icon className="mr-2 h-3.5 w-3.5" />
          {option.label}
        </DropdownMenuItem>
      ))}
    </DropdownMenuContent>
  </DropdownMenu>
);
