"use client";

import { type LucideIcon } from "lucide-react";

import { Badge } from "@/components/ui/badge";
import { Button } from "@/components/ui/button";
import {
  DropdownMenu,
  DropdownMenuCheckboxItem,
  DropdownMenuContent,
  DropdownMenuLabel,
  DropdownMenuSeparator,
  DropdownMenuTrigger
} from "@/components/ui/dropdown-menu";

export type FilterOption<T extends string> = {
  value: T;
  label: string;
};

type FilterDropdownProps<T extends string> = {
  icon: LucideIcon;
  label: string;
  selectedValues: Set<T>;
  options: FilterOption<T>[];
  onToggle: (value: T) => void;
  getSummary?: (selected: Set<T>) => string;
  menuWidth?: string;
};

export const FilterDropdown = <T extends string>({
  icon: Icon,
  label,
  selectedValues,
  options,
  onToggle,
  getSummary,
  menuWidth = "w-40"
}: FilterDropdownProps<T>) => {
  const summary = (() => {
    if (getSummary) return getSummary(selectedValues);
    if (selectedValues.size === 0) return label;
    const selected = Array.from(selectedValues);
    const first = options.find((o) => o.value === selected[0])?.label ?? selected[0];
    if (selectedValues.size === 1) return `${label}: ${first}`;
    return `${label}: ${first} +${selectedValues.size - 1}`;
  })();

  return (
    <DropdownMenu>
      <DropdownMenuTrigger asChild>
        <Button variant="outline" size="sm" className="h-9 gap-1.5 xl:h-8 2xl:h-9">
          <Icon className="h-3.5 w-3.5 2xl:h-4 2xl:w-4" />
          <span className="text-xs 2xl:text-sm">{summary}</span>
          {selectedValues.size > 0 && (
            <Badge variant="secondary" className="h-4 min-w-4 rounded-full px-1 text-[10px] 2xl:h-5 2xl:text-xs">
              {selectedValues.size}
            </Badge>
          )}
        </Button>
      </DropdownMenuTrigger>
      <DropdownMenuContent align="end" className={menuWidth}>
        <DropdownMenuLabel className="text-xs">{label}</DropdownMenuLabel>
        <DropdownMenuSeparator />
        {options.map((option) => (
          <DropdownMenuCheckboxItem
            key={option.value}
            checked={selectedValues.has(option.value)}
            onCheckedChange={() => onToggle(option.value)}
            className="text-xs"
          >
            {option.label}
          </DropdownMenuCheckboxItem>
        ))}
      </DropdownMenuContent>
    </DropdownMenu>
  );
};
