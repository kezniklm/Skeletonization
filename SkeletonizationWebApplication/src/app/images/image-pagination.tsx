"use client";

import { ChevronLeft, ChevronRight } from "lucide-react";

import { Button } from "@/components/ui/button";

type ImagePaginationProps = {
  currentPage: number;
  totalPages: number;
  onPageChange: (page: number) => void;
};

export const ImagePagination = ({ currentPage, totalPages, onPageChange }: ImagePaginationProps) => {
  const getPageNumbers = () => {
    const pages: (number | "ellipsis")[] = [];
    const maxVisible = 5;

    if (totalPages <= maxVisible) {
      for (let i = 1; i <= totalPages; i++) {
        pages.push(i);
      }
      return pages;
    }

    pages.push(1);

    if (currentPage > 3) {
      pages.push("ellipsis");
    }

    const start = Math.max(2, currentPage - 1);
    const end = Math.min(totalPages - 1, currentPage + 1);

    for (let i = start; i <= end; i++) {
      pages.push(i);
    }

    if (currentPage < totalPages - 2) {
      pages.push("ellipsis");
    }

    if (totalPages > 1) {
      pages.push(totalPages);
    }

    return pages;
  };

  return (
    <div className="flex items-center gap-1">
      <Button
        variant="outline"
        size="sm"
        onClick={() => onPageChange(currentPage - 1)}
        disabled={currentPage === 1}
        aria-label="Previous page"
        className="h-8 w-8 p-0 xl:h-7 xl:w-7 2xl:h-8 2xl:w-8"
      >
        <ChevronLeft className="h-3.5 w-3.5 2xl:h-4 2xl:w-4" />
      </Button>

      <div className="flex gap-0.5">
        {getPageNumbers().map((page, index) =>
          page === "ellipsis" ? (
            <div
              key={`ellipsis-${index}`}
              className="text-muted-foreground flex h-8 w-8 items-center justify-center text-xs xl:h-7 xl:w-7 2xl:h-8 2xl:w-8 2xl:text-sm"
            >
              ...
            </div>
          ) : (
            <Button
              key={page}
              variant={currentPage === page ? "default" : "outline"}
              size="sm"
              onClick={() => onPageChange(page)}
              className="h-8 w-8 p-0 text-xs xl:h-7 xl:w-7 2xl:h-8 2xl:w-8 2xl:text-sm"
            >
              {page}
            </Button>
          )
        )}
      </div>

      <Button
        variant="outline"
        size="sm"
        onClick={() => onPageChange(currentPage + 1)}
        disabled={currentPage === totalPages}
        aria-label="Next page"
        className="h-8 w-8 p-0 xl:h-7 xl:w-7 2xl:h-8 2xl:w-8"
      >
        <ChevronRight className="h-3.5 w-3.5 2xl:h-4 2xl:w-4" />
      </Button>
    </div>
  );
};
