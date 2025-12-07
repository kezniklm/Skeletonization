"use client";

import {
  Code2,
  Download,
  Eye,
  EyeOff,
  Move,
  Pencil,
  RotateCcw,
  RotateCw,
  Save,
  Sliders,
  Sparkles,
  Trash2
} from "lucide-react";
import { type Dispatch, type SetStateAction } from "react";

import { Card, CardContent, CardDescription, CardHeader, CardTitle } from "@/components/ui/card";
import { Tabs, TabsContent, TabsList, TabsTrigger } from "@/components/ui/tabs";
import { Button } from "@/components/ui/button";
import { Separator } from "@/components/ui/separator";
import { Tooltip, TooltipContent, TooltipProvider, TooltipTrigger } from "@/components/ui/tooltip";
import { Select, SelectContent, SelectItem, SelectTrigger, SelectValue } from "@/components/ui/select";

import { type FilterState, type TransformState, type DrawingTool } from "../types";

import { CodeTab } from "./(tabs)/code-tab";
import { DrawTab } from "./(tabs)/draw-tab";
import { FiltersTab } from "./(tabs)/filters-tab";
import { TransformTab } from "./(tabs)/transforms-tab";

type ControlsPanelProps = {
  filters: FilterState;
  setFilters: Dispatch<SetStateAction<FilterState>>;
  transforms: TransformState;
  setTransforms: Dispatch<SetStateAction<TransformState>>;
  onResetFilters: () => void;
  onResetTransforms: () => void;
  onResetAll: () => void;
  onApplyPreset: (preset: string) => void;
  historyIndex: number;
  historyLength: number;
  onUndo: () => void;
  onRedo: () => void;
  showComparison: boolean;
  onToggleComparison: () => void;
  onDownload: () => void;
  onSave: () => void;
  saving: boolean;
  activeTool: DrawingTool;
  setActiveTool: Dispatch<SetStateAction<DrawingTool>>;
  drawColor: string;
  setDrawColor: Dispatch<SetStateAction<string>>;
  brushSize: number;
  setBrushSize: Dispatch<SetStateAction<number>>;
  customCode: string;
  setCustomCode: Dispatch<SetStateAction<string>>;
  codeError: string | null;
  onExecuteCode: () => void;
  processing: boolean;
  onAddHistory: (description: string) => void;
};

export const ControlsPanel = ({
  filters,
  setFilters,
  transforms,
  setTransforms,
  onResetFilters,
  onResetTransforms,
  onResetAll,
  onApplyPreset,
  historyIndex,
  historyLength,
  onUndo,
  onRedo,
  showComparison,
  onToggleComparison,
  onDownload,
  onSave,
  saving,
  activeTool,
  setActiveTool,
  drawColor,
  setDrawColor,
  brushSize,
  setBrushSize,
  customCode,
  setCustomCode,
  codeError,
  onExecuteCode,
  processing,
  onAddHistory
}: ControlsPanelProps) => (
  <Card className="flex h-full flex-col gap-0 overflow-hidden border-gray-200 bg-white py-0 shadow-sm backdrop-blur-sm dark:border-gray-800 dark:bg-gray-900/95">
    <CardHeader className="shrink-0 space-y-3 border-b border-gray-200 bg-linear-to-r from-cyan-50/50 to-blue-50/50 px-6 py-3 dark:border-gray-800 dark:from-cyan-950/20 dark:to-blue-950/20">
      <div className="flex items-center space-x-3">
        <div className="flex h-10 w-10 items-center justify-center rounded-lg bg-linear-to-r from-cyan-500 to-blue-500 text-white">
          <Move className="size-5" />
        </div>
        <div>
          <CardTitle className="text-lg font-semibold text-gray-900 dark:text-white">Processing Controls</CardTitle>
          <CardDescription className="text-sm text-gray-600 dark:text-gray-400">
            Adjust filters, transforms, and operations
          </CardDescription>
        </div>
      </div>

      <TooltipProvider>
        <div className="space-y-2">
          <div className="flex items-center gap-1.5">
            <div className="flex items-center gap-1">
              <Tooltip>
                <TooltipTrigger asChild>
                  <Button
                    variant="outline"
                    size="icon"
                    className="size-7"
                    onClick={onUndo}
                    disabled={historyIndex <= 0}
                  >
                    <RotateCcw className="size-3.5" />
                  </Button>
                </TooltipTrigger>
                <TooltipContent>
                  <p>Undo</p>
                </TooltipContent>
              </Tooltip>

              <Tooltip>
                <TooltipTrigger asChild>
                  <Button
                    variant="outline"
                    size="icon"
                    className="size-7"
                    onClick={onRedo}
                    disabled={historyIndex >= historyLength - 1}
                  >
                    <RotateCw className="size-3.5" />
                  </Button>
                </TooltipTrigger>
                <TooltipContent>
                  <p>Redo</p>
                </TooltipContent>
              </Tooltip>

              <Tooltip>
                <TooltipTrigger asChild>
                  <Button variant="outline" size="icon" className="size-7" onClick={onResetAll}>
                    <Trash2 className="size-3.5" />
                  </Button>
                </TooltipTrigger>
                <TooltipContent>
                  <p>Reset All</p>
                </TooltipContent>
              </Tooltip>
            </div>

            <Separator orientation="vertical" className="h-5" />

            <Tooltip>
              <TooltipTrigger asChild>
                <Button variant="outline" size="icon" className="size-7" onClick={onToggleComparison}>
                  {showComparison ? <EyeOff className="size-3.5" /> : <Eye className="size-3.5" />}
                </Button>
              </TooltipTrigger>
              <TooltipContent>
                <p>{showComparison ? "Hide" : "Show"} Comparison</p>
              </TooltipContent>
            </Tooltip>

            <Select value="" onValueChange={onApplyPreset}>
              <SelectTrigger className="h-7 flex-1 text-xs">
                <SelectValue placeholder="Apply Preset..." />
              </SelectTrigger>
              <SelectContent>
                <SelectItem value="enhance">
                  <div className="flex items-center gap-2">
                    <Sparkles className="size-3.5" />
                    <span className="text-xs">Enhance</span>
                  </div>
                </SelectItem>
                <SelectItem value="bw">
                  <div className="flex items-center gap-2">
                    <Sliders className="size-3.5" />
                    <span className="text-xs">Black &amp; White</span>
                  </div>
                </SelectItem>
                <SelectItem value="smooth">
                  <div className="flex items-center gap-2">
                    <Sliders className="size-3.5" />
                    <span className="text-xs">Smooth</span>
                  </div>
                </SelectItem>
                <SelectItem value="vivid">
                  <div className="flex items-center gap-2">
                    <Sparkles className="size-3.5" />
                    <span className="text-xs">Vivid</span>
                  </div>
                </SelectItem>
              </SelectContent>
            </Select>
          </div>

          <div className="flex w-full gap-1.5">
            <Tooltip>
              <TooltipTrigger asChild>
                <Button variant="outline" size="sm" className="flex-1 text-xs" onClick={onDownload}>
                  <Download className="mr-1.5 size-3.5" />
                  Download
                </Button>
              </TooltipTrigger>
              <TooltipContent>
                <p>Download processed image</p>
              </TooltipContent>
            </Tooltip>
            <Tooltip>
              <TooltipTrigger asChild>
                <Button size="sm" className="flex-1 text-xs" onClick={onSave} disabled={saving}>
                  <Save className="mr-1.5 size-3.5" />
                  {saving ? "Saving..." : "Save"}
                </Button>
              </TooltipTrigger>
              <TooltipContent>
                <p>Save to library</p>
              </TooltipContent>
            </Tooltip>
          </div>
        </div>
      </TooltipProvider>
    </CardHeader>

    <CardContent className="min-h-0 flex-1 overflow-hidden px-6 py-4">
      <Tabs defaultValue="filters" className="flex h-full flex-col">
        <TabsList className="grid w-full shrink-0 grid-cols-4 bg-linear-to-r from-cyan-100/50 to-blue-100/50 dark:from-cyan-950/30 dark:to-blue-950/30">
          <TabsTrigger
            value="filters"
            className="data-[state=active]:bg-linear-to-r data-[state=active]:from-cyan-500 data-[state=active]:to-blue-500 data-[state=active]:text-white"
          >
            <Sliders className="mr-2 size-4" />
            Filters
          </TabsTrigger>
          <TabsTrigger
            value="draw"
            className="data-[state=active]:bg-linear-to-r data-[state=active]:from-cyan-500 data-[state=active]:to-blue-500 data-[state=active]:text-white"
          >
            <Pencil className="mr-2 size-4" />
            Draw
          </TabsTrigger>
          <TabsTrigger
            value="transform"
            className="data-[state=active]:bg-linear-to-r data-[state=active]:from-cyan-500 data-[state=active]:to-blue-500 data-[state=active]:text-white"
          >
            <RotateCw className="mr-2 size-4" />
            Transform
          </TabsTrigger>
          <TabsTrigger
            value="code"
            className="data-[state=active]:bg-linear-to-r data-[state=active]:from-cyan-500 data-[state=active]:to-blue-500 data-[state=active]:text-white"
          >
            <Code2 className="mr-2 size-4" />
            Code
          </TabsTrigger>
        </TabsList>

        <TabsContent value="filters" className="mt-0 min-h-0 flex-1 overflow-y-auto pr-2">
          <FiltersTab
            filters={filters}
            setFilters={setFilters}
            onResetFilters={onResetFilters}
            onAddHistory={onAddHistory}
          />
        </TabsContent>

        <TabsContent value="draw" className="mt-0 min-h-0 flex-1 overflow-y-auto pr-2">
          <DrawTab
            activeTool={activeTool}
            setActiveTool={setActiveTool}
            drawColor={drawColor}
            setDrawColor={setDrawColor}
            brushSize={brushSize}
            setBrushSize={setBrushSize}
          />
        </TabsContent>

        <TabsContent value="transform" className="mt-0 min-h-0 flex-1 overflow-y-auto pr-2">
          <TransformTab
            transforms={transforms}
            setTransforms={setTransforms}
            onResetTransforms={onResetTransforms}
            onAddHistory={onAddHistory}
          />
        </TabsContent>

        <TabsContent value="code" className="mt-0 min-h-0 flex-1 overflow-y-auto pr-2">
          <CodeTab
            customCode={customCode}
            setCustomCode={setCustomCode}
            codeError={codeError}
            onExecuteCode={onExecuteCode}
            processing={processing}
          />
        </TabsContent>
      </Tabs>
    </CardContent>
  </Card>
);
