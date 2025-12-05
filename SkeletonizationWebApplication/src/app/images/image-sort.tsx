"use client";

import { ArrowDownAZ, ArrowUpAZ, Calendar, ArrowUpDown } from "lucide-react";

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

const sortOptions = [
  { value: "date-desc" as const, label: "Newest First", icon: Calendar },
  { value: "date-asc" as const, label: "Oldest First", icon: Calendar },
  { value: "name-asc" as const, label: "Name (A-Z)", icon: ArrowDownAZ },
  { value: "name-desc" as const, label: "Name (Z-A)", icon: ArrowUpAZ }
];

export const ImageSort = ({ sortBy, onSortChange }: ImageSortProps) => (
  <DropdownMenu>
    <DropdownMenuTrigger asChild>
      <Button variant="outline" size="sm" className="h-9 gap-1.5 xl:h-8 2xl:h-9">
        <ArrowUpDown className="h-3.5 w-3.5 2xl:h-4 2xl:w-4" />
        <span className="hidden text-xs lg:inline 2xl:text-sm">Sort</span>
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
