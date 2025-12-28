"use client";

import { useState } from "react";
import { Minus, Plus, RotateCcw, SplitSquareVertical } from "lucide-react";
import Image from "next/image";

import { Button } from "@/components/ui/button";
import { Dialog, DialogContent, DialogDescription, DialogHeader, DialogTitle } from "@/components/ui/dialog";
import { Slider } from "@/components/ui/slider";
import { Tabs, TabsContent, TabsList, TabsTrigger } from "@/components/ui/tabs";
import { cn } from "@/lib/utils";

import { usePanZoom } from "./use-pan-zoom";

export type ViewerSelection = "original" | "output";

type JobViewerDialogProps = {
  open: boolean;
  onOpenChange: (open: boolean) => void;
  title: string;
  originalSrc: string;
  selectedSrc: string;
  selection: ViewerSelection;
};

type CompareMode = "single" | "side-by-side" | "slider" | "overlay";

export const JobViewerDialog = ({
  open,
  onOpenChange,
  title,
  originalSrc,
  selectedSrc,
  selection
}: JobViewerDialogProps) => {
  const hasOutput = selection === "output" && Boolean(selectedSrc) && Boolean(originalSrc);

  const [mode, setMode] = useState<CompareMode>(hasOutput ? "slider" : "single");
  const {
    zoom,
    isDragging,
    stageTransform,
    zoomIn,
    zoomOut,
    resetView,
    onWheel,
    onPointerDown,
    onPointerMove,
    stopDragging
  } = usePanZoom();

  const [reveal, setReveal] = useState(50);
  const [overlayOpacity, setOverlayOpacity] = useState(55);

  const singleSrc = selection === "original" ? originalSrc : selectedSrc;

  return (
    <Dialog open={open} onOpenChange={onOpenChange}>
      <DialogContent className="max-w-[calc(100%-2rem)] sm:max-w-5xl">
        <DialogHeader>
          <DialogTitle className="truncate">{title || "Image"}</DialogTitle>
          <DialogDescription>Drag to pan • Use controls to zoom</DialogDescription>
        </DialogHeader>

        <div className="flex flex-col gap-3">
          <div className="flex flex-wrap items-center justify-between gap-2">
            <div className="flex flex-wrap items-center gap-2">
              <Button type="button" variant="outline" size="sm" onClick={zoomOut}>
                <Minus />
                Zoom out
              </Button>
              <Button type="button" variant="outline" size="sm" onClick={zoomIn}>
                <Plus />
                Zoom in
              </Button>
              <Button type="button" variant="outline" size="sm" onClick={resetView}>
                <RotateCcw />
                Reset
              </Button>
              <span className="text-xs text-gray-600 dark:text-gray-400">{Math.round(zoom * 100)}%</span>
            </div>

            {hasOutput && (
              <div className="flex items-center gap-2 text-xs text-gray-600 dark:text-gray-400">
                <SplitSquareVertical className="h-4 w-4" />
                Compare modes available
              </div>
            )}
          </div>

          <Tabs value={mode} onValueChange={(v) => setMode(v as CompareMode)} className="gap-3">
            <TabsList className="w-full justify-start">
              <TabsTrigger value="single">Single</TabsTrigger>
              {hasOutput && <TabsTrigger value="side-by-side">Side-by-side</TabsTrigger>}
              {hasOutput && <TabsTrigger value="slider">Slider</TabsTrigger>}
              {hasOutput && <TabsTrigger value="overlay">Overlay</TabsTrigger>}
            </TabsList>

            <div
              className={cn(
                "relative h-[65vh] w-full touch-none overflow-hidden rounded-xl border border-gray-200 bg-gray-50 select-none dark:border-gray-800 dark:bg-gray-900",
                isDragging ? "cursor-grabbing" : "cursor-grab"
              )}
              onPointerDown={onPointerDown}
              onPointerMove={onPointerMove}
              onPointerUp={stopDragging}
              onPointerCancel={stopDragging}
              onPointerLeave={stopDragging}
              onWheel={onWheel}
            >
              <div
                className="absolute inset-0"
                style={{
                  transform: stageTransform,
                  transformOrigin: "center",
                  willChange: "transform"
                }}
              >
                <TabsContent value="single" className="h-full">
                  <div className="flex h-full w-full items-center justify-center">
                    <Image
                      src={singleSrc}
                      alt={title}
                      width={1600}
                      height={1200}
                      draggable={false}
                      className="max-h-full max-w-full object-contain"
                    />
                  </div>
                </TabsContent>

                {hasOutput && (
                  <>
                    <TabsContent value="side-by-side" className="h-full">
                      <div className="grid h-full w-full grid-cols-1 gap-2 p-2 sm:grid-cols-2">
                        <div className="flex items-center justify-center overflow-hidden rounded-lg bg-black/5 dark:bg-white/5">
                          <Image
                            src={originalSrc}
                            alt={`${title} original`}
                            width={1600}
                            height={1200}
                            draggable={false}
                            className="max-h-full max-w-full object-contain"
                          />
                        </div>
                        <div className="flex items-center justify-center overflow-hidden rounded-lg bg-black/5 dark:bg-white/5">
                          <Image
                            src={selectedSrc}
                            alt={`${title} output`}
                            width={1600}
                            height={1200}
                            draggable={false}
                            className="max-h-full max-w-full object-contain"
                          />
                        </div>
                      </div>
                    </TabsContent>

                    <TabsContent value="slider" className="h-full">
                      <div className="relative h-full w-full">
                        <div className="absolute inset-0 flex items-center justify-center">
                          <Image
                            src={originalSrc}
                            alt={`${title} original`}
                            width={1600}
                            height={1200}
                            draggable={false}
                            className="max-h-full max-w-full object-contain"
                          />
                        </div>

                        <div
                          className="absolute inset-0 flex items-center justify-center overflow-hidden"
                          style={{ clipPath: `inset(0 ${100 - reveal}% 0 0)` }}
                        >
                          <Image
                            src={selectedSrc}
                            alt={`${title} output`}
                            width={1600}
                            height={1200}
                            draggable={false}
                            className="max-h-full max-w-full object-contain"
                          />
                        </div>

                        <div className="absolute inset-x-0 bottom-4 mx-auto w-[min(28rem,90%)] rounded-lg border border-gray-200 bg-white/90 p-3 backdrop-blur dark:border-gray-800 dark:bg-gray-950/90">
                          <p className="mb-2 text-xs font-medium text-gray-700 dark:text-gray-300">Reveal</p>
                          <Slider
                            value={[reveal]}
                            onValueChange={(v) => setReveal(v[0] ?? 50)}
                            min={0}
                            max={100}
                            step={1}
                          />
                        </div>
                      </div>
                    </TabsContent>

                    <TabsContent value="overlay" className="h-full">
                      <div className="relative h-full w-full">
                        <div className="absolute inset-0 flex items-center justify-center">
                          <Image
                            src={originalSrc}
                            alt={`${title} original`}
                            width={1600}
                            height={1200}
                            draggable={false}
                            className="max-h-full max-w-full object-contain"
                          />
                        </div>

                        <div
                          className="absolute inset-0 flex items-center justify-center"
                          style={{ opacity: overlayOpacity / 100 }}
                        >
                          <Image
                            src={selectedSrc}
                            alt={`${title} output`}
                            width={1600}
                            height={1200}
                            draggable={false}
                            className="max-h-full max-w-full object-contain"
                          />
                        </div>

                        <div className="absolute inset-x-0 bottom-4 mx-auto w-[min(28rem,90%)] rounded-lg border border-gray-200 bg-white/90 p-3 backdrop-blur dark:border-gray-800 dark:bg-gray-950/90">
                          <p className="mb-2 text-xs font-medium text-gray-700 dark:text-gray-300">Overlay opacity</p>
                          <Slider
                            value={[overlayOpacity]}
                            onValueChange={(v) => setOverlayOpacity(v[0] ?? 55)}
                            min={0}
                            max={100}
                            step={1}
                          />
                        </div>
                      </div>
                    </TabsContent>
                  </>
                )}
              </div>
            </div>
          </Tabs>

          {hasOutput && (
            <div className="flex flex-wrap items-center justify-between gap-2 text-xs text-gray-600 dark:text-gray-400">
              <div className="flex items-center gap-2">
                <span className="inline-flex items-center gap-1 rounded-full bg-gray-100 px-2 py-0.5 dark:bg-gray-800">
                  Original
                </span>
                <span className="inline-flex items-center gap-1 rounded-full bg-gray-100 px-2 py-0.5 dark:bg-gray-800">
                  Output
                </span>
              </div>
              <span>Select compare mode to view both</span>
            </div>
          )}
        </div>
      </DialogContent>
    </Dialog>
  );
};
