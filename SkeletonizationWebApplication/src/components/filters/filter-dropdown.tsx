/**
 * @file filter-dropdown.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Provides generic multi-select filter dropdown UI.
 * @description Renders selectable filter options with summary text and selected-count badge.
 * @version 1.0
 * @date 2026-03-16
 */

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

/**
 * @brief Defines one selectable filter option.
 * @description Represents typed value-label pairs used by the generic filter dropdown.
 */
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

/**
 * @brief Renders a typed filter dropdown with checkbox items.
 * @description Shows a summarized trigger label and allows toggling multiple filter options.
 * @param icon Icon component displayed on the trigger.
 * @param label Base label of the filter group.
 * @param selectedValues Set of selected option values.
 * @param options Available options for selection.
 * @param onToggle Callback to toggle an option value.
 * @param getSummary Optional custom summary text generator.
 * @param menuWidth Width class applied to dropdown content.
 * @returns A dropdown filter selector component.
 */
/** @brief Renders a typed multi-select filter dropdown. */
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
