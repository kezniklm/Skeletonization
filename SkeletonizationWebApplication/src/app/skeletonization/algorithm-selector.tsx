"use client";

import { Check, ChevronDown } from "lucide-react";

import { ALGORITHMS } from "@/algorithms";
import { Card, CardContent, CardHeader, CardTitle } from "@/components/ui/card";
import {
  DropdownMenu,
  DropdownMenuContent,
  DropdownMenuItem,
  DropdownMenuTrigger
} from "@/components/ui/dropdown-menu";
import { Button } from "@/components/ui/button";

type AlgorithmSelectorProps = {
  selectedAlgorithm: string | null;
  onAlgorithmChange: (algorithm: string) => void;
};

export const AlgorithmSelector = ({ selectedAlgorithm, onAlgorithmChange }: AlgorithmSelectorProps) => (
  <Card>
    <CardHeader>
      <CardTitle className="text-lg">Select Algorithm</CardTitle>
    </CardHeader>
    <CardContent>
      <DropdownMenu>
        <DropdownMenuTrigger asChild>
          <Button variant="outline" className="w-full justify-between">
            {selectedAlgorithm ?? "Choose an algorithm..."}
            <ChevronDown className="ml-2 h-4 w-4 opacity-50" />
          </Button>
        </DropdownMenuTrigger>
        <DropdownMenuContent className="w-(--radix-dropdown-menu-trigger-width)">
          {ALGORITHMS.map((algo) => (
            <DropdownMenuItem key={algo} onSelect={() => onAlgorithmChange(algo)}>
              <div className="flex w-full items-center justify-between">
                <span>{algo}</span>
                {selectedAlgorithm === algo && <Check className="h-4 w-4 text-cyan-600 dark:text-cyan-400" />}
              </div>
            </DropdownMenuItem>
          ))}
        </DropdownMenuContent>
      </DropdownMenu>

      {selectedAlgorithm && (
        <div className="mt-4 rounded-lg border border-cyan-200 bg-cyan-50 p-3 dark:border-cyan-800 dark:bg-cyan-950/20">
          <p className="text-sm font-medium text-cyan-900 dark:text-cyan-100">Selected: {selectedAlgorithm}</p>
          <p className="mt-1 text-xs text-cyan-700 dark:text-cyan-300">
            This algorithm will be applied to all selected images.
          </p>
        </div>
      )}
    </CardContent>
  </Card>
);
