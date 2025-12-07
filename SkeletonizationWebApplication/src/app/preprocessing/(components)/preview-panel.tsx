"use client";

import { Eye, Image as ImageIcon } from "lucide-react";
import { type RefObject, useEffect, useRef, useState, type MouseEvent, type CSSProperties } from "react";

import { Card, CardContent, CardDescription, CardHeader, CardTitle } from "@/components/ui/card";
import { Button } from "@/components/ui/button";
import { Tooltip, TooltipContent, TooltipProvider, TooltipTrigger } from "@/components/ui/tooltip";

import { type DrawingTool } from "../types";

type PreviewPanelProps = {
  canvasRef: RefObject<HTMLCanvasElement>;
  comparisonCanvasRef: RefObject<HTMLCanvasElement>;
  showComparison: boolean;
  activeTool: DrawingTool;
  onChangeImage: () => void;
  onMouseDown: (e: MouseEvent<HTMLCanvasElement>) => void;
  onMouseMove: (e: MouseEvent<HTMLCanvasElement>) => void;
  onMouseUp: (e: MouseEvent<HTMLCanvasElement>) => void;
  onMouseLeave: (e: MouseEvent<HTMLCanvasElement>) => void;
};

const calculateScale = (
  canvas: HTMLCanvasElement | null,
  container: HTMLDivElement | null
): { width: string; height: string } => {
  if (!canvas || !container) {
    return { width: "auto", height: "auto" };
  }

  const canvasWidth = canvas.width;
  const canvasHeight = canvas.height;
  if (canvasWidth === 0 || canvasHeight === 0) return { width: "auto", height: "auto" };

  const containerRect = container.getBoundingClientRect();
  const containerWidth = containerRect.width - 16;
  const containerHeight = containerRect.height - 16;

  if (containerWidth <= 0 || containerHeight <= 0) return { width: "auto", height: "auto" };

  const canvasAspect = canvasWidth / canvasHeight;
  const containerAspect = containerWidth / containerHeight;

  return canvasAspect > containerAspect
    ? { width: `${containerWidth}px`, height: `${containerWidth / canvasAspect}px` }
    : { width: `${containerHeight * canvasAspect}px`, height: `${containerHeight}px` };
};

export const PreviewPanel = ({
  canvasRef,
  comparisonCanvasRef,
  showComparison,
  activeTool,
  onChangeImage,
  onMouseDown,
  onMouseMove,
  onMouseUp,
  onMouseLeave
}: PreviewPanelProps) => {
  const containerRef = useRef<HTMLDivElement>(null);
  const comparisonContainerRef = useRef<HTMLDivElement>(null);
  const [canvasScale, setCanvasScale] = useState({ width: "auto", height: "auto" });
  const [comparisonScale, setComparisonScale] = useState({ width: "auto", height: "auto" });

  useEffect(() => {
    const updateScales = () => {
      setCanvasScale(calculateScale(canvasRef.current, containerRef.current));
      setComparisonScale(calculateScale(comparisonCanvasRef.current, comparisonContainerRef.current));
    };

    updateScales();

    const canvas = canvasRef.current;
    const compCanvas = comparisonCanvasRef.current;

    const observer = new MutationObserver(updateScales);
    if (canvas) {
      observer.observe(canvas, { attributes: true, attributeFilter: ["width", "height"] });
    }
    if (compCanvas) {
      observer.observe(compCanvas, { attributes: true, attributeFilter: ["width", "height"] });
    }

    const resizeObserver = new ResizeObserver(updateScales);
    if (containerRef.current) resizeObserver.observe(containerRef.current);
    if (comparisonContainerRef.current) resizeObserver.observe(comparisonContainerRef.current);

    const interval = setInterval(updateScales, 200);

    return () => {
      observer.disconnect();
      resizeObserver.disconnect();
      clearInterval(interval);
    };
  }, [canvasRef, comparisonCanvasRef, showComparison]);

  const getCanvasStyle = (scale: { width: string; height: string }): CSSProperties => ({
    cursor: activeTool !== "none" ? "crosshair" : "default",
    width: scale.width,
    height: scale.height,
    display: "block"
  });

  return (
    <Card className="flex h-full flex-col gap-0 overflow-hidden border-gray-200 bg-white py-0 shadow-sm backdrop-blur-sm dark:border-gray-800 dark:bg-gray-900/95">
      <CardHeader className="shrink-0 border-b border-gray-200 bg-linear-to-r from-cyan-50/50 to-blue-50/50 px-6 py-3 dark:border-gray-800 dark:from-cyan-950/20 dark:to-blue-950/20 [.border-b]:pb-2">
        <div className="flex items-center justify-between">
          <div className="flex items-center space-x-3">
            <div className="flex h-10 w-10 items-center justify-center rounded-lg bg-linear-to-r from-cyan-500 to-blue-500 text-white">
              <Eye className="size-5" />
            </div>
            <div>
              <CardTitle className="text-lg font-semibold text-gray-900 dark:text-white">Live Preview</CardTitle>
              <CardDescription className="text-sm text-gray-600 dark:text-gray-400">
                Changes apply in real-time
              </CardDescription>
            </div>
          </div>
          <TooltipProvider>
            <Tooltip>
              <TooltipTrigger asChild>
                <Button variant="ghost" size="sm" onClick={onChangeImage}>
                  <ImageIcon className="size-4" />
                  <span className="ml-2 hidden sm:inline">Change</span>
                </Button>
              </TooltipTrigger>
              <TooltipContent>
                <p>Select a different image</p>
              </TooltipContent>
            </Tooltip>
          </TooltipProvider>
        </div>
      </CardHeader>

      <CardContent className="bg-muted/20 flex min-h-0 flex-1 flex-col p-0">
        {showComparison ? (
          <div className="grid min-h-0 flex-1 gap-3 p-3 lg:grid-cols-2">
            <div className="bg-background flex min-h-0 flex-col overflow-hidden rounded-lg border">
              <div
                className="shrink-0 border-b px-3 py-1.5 text-center text-xs font-medium text-white"
                style={{ backgroundColor: "var(--primary)" }}
              >
                Original
              </div>
              <div
                ref={comparisonContainerRef}
                className="flex min-h-0 flex-1 items-center justify-center overflow-hidden p-2"
              >
                <canvas ref={comparisonCanvasRef} style={getCanvasStyle(comparisonScale)} />
              </div>
            </div>
            <div className="bg-background flex min-h-0 flex-col overflow-hidden rounded-lg border">
              <div
                className="shrink-0 border-b px-3 py-1.5 text-center text-xs font-medium text-white"
                style={{ backgroundColor: "var(--primary)" }}
              >
                Processed
              </div>
              <div ref={containerRef} className="flex min-h-0 flex-1 items-center justify-center overflow-hidden p-2">
                <canvas
                  ref={canvasRef}
                  onMouseDown={onMouseDown}
                  onMouseMove={onMouseMove}
                  onMouseUp={onMouseUp}
                  onMouseLeave={onMouseLeave}
                  style={getCanvasStyle(canvasScale)}
                />
              </div>
            </div>
          </div>
        ) : (
          <div
            ref={containerRef}
            className="bg-background flex min-h-0 flex-1 items-center justify-center overflow-hidden p-2"
          >
            <canvas
              ref={canvasRef}
              onMouseDown={onMouseDown}
              onMouseMove={onMouseMove}
              onMouseUp={onMouseUp}
              onMouseLeave={onMouseLeave}
              style={getCanvasStyle(canvasScale)}
            />
          </div>
        )}
      </CardContent>
    </Card>
  );
};
