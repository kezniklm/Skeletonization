/**
 * @file clear-filters-button.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Renders a conditional button for clearing active filters.
 * @description Displays a compact clear action only when at least one filter is active.
 * @version 1.0
 * @date 2026-03-16
 */

"use client";

import { X } from "lucide-react";

import { Button } from "@/components/ui/button";

type ClearFiltersButtonProps = {
  onClick: () => void;
  show: boolean;
};

/**
 * @brief Displays a clear-filters action button.
 * @description Returns `null` when hidden; otherwise renders a button that resets active filters.
 * @param onClick Callback executed when the button is pressed.
 * @param show Whether the button should be visible.
 * @returns Clear button element or `null`.
 */
export const ClearFiltersButton = ({ onClick, show }: ClearFiltersButtonProps) => {
  if (!show) {
    return null;
  }

  return (
    <Button
      variant="ghost"
      size="sm"
      onClick={onClick}
      className="h-9 gap-1.5 xl:h-8 2xl:h-9"
      title="Clear all filters"
    >
      <X className="h-3.5 w-3.5 2xl:h-4 2xl:w-4" />
      <span className="text-xs 2xl:text-sm">Clear</span>
    </Button>
  );
};
