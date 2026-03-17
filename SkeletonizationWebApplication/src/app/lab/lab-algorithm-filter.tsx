/**
 * @file lab-algorithm-filter.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Implements algorithm filter dropdown for lab history.
 * @description Allows multi-select algorithm filtering using checkbox menu items.
 * @version 1.0
 * @date 2026-03-16
 */

"use client";

import { Filter } from "lucide-react";

import { type Algorithm } from "@/algorithms";
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
import { formatAlgorithmName } from "@/lib/format";

/**
 * @brief Represents algorithm filter component properties.
 */
type LabAlgorithmFilterProps = {
  readonly algorithms: readonly Algorithm[];
  selectedAlgorithms: ReadonlySet<Algorithm>;
  toggleAlgorithm: (algorithm: Algorithm) => void;
};

/**
 * @brief Renders algorithm multi-select dropdown.
 * @param algorithms Available algorithm list.
 * @param selectedAlgorithms Currently selected algorithms.
 * @param toggleAlgorithm Callback to toggle algorithm selection.
 * @returns Dropdown filter JSX.
 */
export const LabAlgorithmFilter = ({ algorithms, selectedAlgorithms, toggleAlgorithm }: LabAlgorithmFilterProps) => (
  <DropdownMenu>
    <DropdownMenuTrigger asChild>
      <Button variant="outline" size="sm" className="h-9 gap-1.5 xl:h-8 2xl:h-9">
        <Filter className="h-3.5 w-3.5 2xl:h-4 2xl:w-4" />
        <span className="hidden text-xs sm:inline 2xl:text-sm">Algorithm</span>
        {selectedAlgorithms.size > 0 && (
          <Badge variant="secondary" className="h-4 min-w-4 rounded-full px-1 text-[10px] 2xl:h-5 2xl:text-xs">
            {selectedAlgorithms.size}
          </Badge>
        )}
      </Button>
    </DropdownMenuTrigger>

    <DropdownMenuContent align="end" className="w-56">
      <DropdownMenuLabel className="text-xs">Algorithm</DropdownMenuLabel>
      <DropdownMenuSeparator />

      {algorithms.length === 0 ? (
        <div className="px-2 py-1.5 text-xs text-gray-600 dark:text-gray-400">No algorithms</div>
      ) : (
        algorithms.map((algorithm) => (
          <DropdownMenuCheckboxItem
            key={algorithm}
            checked={selectedAlgorithms.has(algorithm)}
            onCheckedChange={() => toggleAlgorithm(algorithm)}
            className="text-xs"
          >
            {formatAlgorithmName(algorithm)}
          </DropdownMenuCheckboxItem>
        ))
      )}
    </DropdownMenuContent>
  </DropdownMenu>
);
