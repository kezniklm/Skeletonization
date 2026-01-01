"use client";

import { X } from "lucide-react";

import { Button } from "@/components/ui/button";

type ClearFiltersButtonProps = {
  onClick: () => void;
  show: boolean;
};

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
