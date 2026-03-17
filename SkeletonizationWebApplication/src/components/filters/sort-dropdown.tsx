/**
 * @file sort-dropdown.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Renders sort selection dropdown controls.
 * @description Provides configurable sort options with iconized labels for image and run listing views.
 * @version 1.0
 * @date 2026-03-16
 */

"use client";

import { ArrowDownAZ, ArrowUpAZ, ArrowUpDown, Calendar, type LucideIcon } from "lucide-react";

import { Button } from "@/components/ui/button";
import {
  DropdownMenu,
  DropdownMenuContent,
  DropdownMenuItem,
  DropdownMenuTrigger
} from "@/components/ui/dropdown-menu";

import { type SortOption } from "./types";

type SortOptionConfig = {
  value: SortOption;
  label: string;
  icon: LucideIcon;
};

const defaultSortOptions: SortOptionConfig[] = [
  { value: "date-desc", label: "Newest First", icon: Calendar },
  { value: "date-asc", label: "Oldest First", icon: Calendar },
  { value: "name-asc", label: "Name (A-Z)", icon: ArrowDownAZ },
  { value: "name-desc", label: "Name (Z-A)", icon: ArrowUpAZ }
];

type SortDropdownProps = {
  value: SortOption;
  onChange: (sort: SortOption) => void;
  options?: SortOptionConfig[];
};

/**
 * @brief Displays a sort option dropdown.
 * @description Renders available sort strategies and notifies parent components when sort selection changes.
 * @param value Currently selected sort option.
 * @param onChange Callback receiving selected sort option.
 * @param options Optional custom option definitions.
 * @returns A dropdown trigger and menu for sorting.
 */
export const SortDropdown = ({ value, onChange, options = defaultSortOptions }: SortDropdownProps) => (
  <DropdownMenu>
    <DropdownMenuTrigger asChild>
      <Button variant="outline" size="sm" className="h-9 gap-1.5 xl:h-8 2xl:h-9">
        <ArrowUpDown className="h-3.5 w-3.5 2xl:h-4 2xl:w-4" />
        <span className="text-xs 2xl:text-sm">{options.find((o) => o.value === value)?.label ?? "Sort"}</span>
      </Button>
    </DropdownMenuTrigger>
    <DropdownMenuContent align="end" className="w-44">
      {options.map((option) => (
        <DropdownMenuItem
          key={option.value}
          onClick={() => onChange(option.value)}
          className={`text-xs ${value === option.value ? "bg-accent" : ""}`}
        >
          <option.icon className="mr-2 h-3.5 w-3.5" />
          {option.label}
        </DropdownMenuItem>
      ))}
    </DropdownMenuContent>
  </DropdownMenu>
);
