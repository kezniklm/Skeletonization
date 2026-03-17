/**
 * @file algorithm-multi-selector.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Renders multi-select controls for skeletonization algorithms.
 * @description Provides algorithm checkbox selection with select-all and deselect-all shortcuts.
 */

"use client";

import { ALGORITHMS, type Algorithm } from "@/algorithms";
import { Badge } from "@/components/ui/badge";
import { Checkbox } from "@/components/ui/checkbox";
import { Label } from "@/components/ui/label";
import { cn } from "@/lib/utils";

type AlgorithmMultiSelectorProps = {
  selectedAlgorithms: Algorithm[];
  onSelectionChange: (algorithms: Algorithm[]) => void;
};

/**
 * @brief Displays algorithm multi-selection controls.
 * @description Lets users choose one or more algorithms and quickly toggle complete selection sets.
 * @param selectedAlgorithms Currently selected algorithm identifiers.
 * @param onSelectionChange Callback receiving updated algorithm selections.
 * @returns Algorithm selection UI with checkboxes.
 */
export const AlgorithmMultiSelector = ({ selectedAlgorithms, onSelectionChange }: AlgorithmMultiSelectorProps) => {
  const toggleAlgorithm = (algorithm: Algorithm) => {
    if (selectedAlgorithms.includes(algorithm)) {
      onSelectionChange(selectedAlgorithms.filter((a) => a !== algorithm));
    } else {
      onSelectionChange([...selectedAlgorithms, algorithm]);
    }
  };

  const selectAll = () => {
    onSelectionChange([...ALGORITHMS]);
  };

  const deselectAll = () => {
    onSelectionChange([]);
  };

  return (
    <div className="space-y-3">
      <div className="flex items-center justify-between">
        <div className="flex items-center gap-2">
          <Label className="text-sm font-medium">Algorithms</Label>
          {selectedAlgorithms.length > 0 && (
            <Badge variant="secondary" className="h-5 px-2 text-xs">
              {selectedAlgorithms.length}
            </Badge>
          )}
        </div>
        <div className="flex gap-2">
          <button
            onClick={selectAll}
            className="text-sm font-medium text-cyan-600 hover:text-cyan-700 dark:text-cyan-400 dark:hover:text-cyan-300"
            type="button"
          >
            Select All
          </button>
          <span className="text-gray-300 dark:text-gray-700">|</span>
          <button
            onClick={deselectAll}
            className="text-sm font-medium text-gray-600 hover:text-gray-700 dark:text-gray-400 dark:hover:text-gray-300"
            type="button"
          >
            Deselect All
          </button>
        </div>
      </div>
      <div className="max-h-40 overflow-auto pb-2">
        <div className="grid grid-cols-3 gap-2">
          {ALGORITHMS.map((algorithm) => {
            const isSelected = selectedAlgorithms.includes(algorithm);
            return (
              <label
                key={algorithm}
                className={cn(
                  "flex cursor-pointer items-center gap-3 rounded-lg border-2 p-3 text-left transition-all",
                  isSelected
                    ? "border-cyan-500 bg-cyan-50 dark:border-cyan-400 dark:bg-cyan-950/30"
                    : "border-gray-200 hover:border-gray-300 hover:bg-gray-50 dark:border-gray-700 dark:hover:border-gray-600 dark:hover:bg-gray-800/50"
                )}
              >
                <Checkbox checked={isSelected} onCheckedChange={() => toggleAlgorithm(algorithm)} />
                <span
                  className={cn(
                    "text-sm font-medium whitespace-nowrap",
                    isSelected ? "text-cyan-900 dark:text-cyan-100" : "text-gray-700 dark:text-gray-300"
                  )}
                >
                  {algorithm}
                </span>
              </label>
            );
          })}
        </div>
      </div>
    </div>
  );
};
