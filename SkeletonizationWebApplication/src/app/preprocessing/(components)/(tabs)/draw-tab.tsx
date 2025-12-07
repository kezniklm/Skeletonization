"use client";

import { Circle, Crop, Eraser, Minus, Move, PaintBucket, Pencil, Square } from "lucide-react";

import { Alert, AlertDescription } from "@/components/ui/alert";
import { Button } from "@/components/ui/button";
import { Input } from "@/components/ui/input";
import { Label } from "@/components/ui/label";
import { Slider } from "@/components/ui/slider";
import { Separator } from "@/components/ui/separator";

import { type DrawingTool } from "../../types";

type DrawTabProps = {
  activeTool: DrawingTool;
  setActiveTool: (tool: DrawingTool) => void;
  drawColor: string;
  setDrawColor: (color: string) => void;
  brushSize: number;
  setBrushSize: (size: number) => void;
};

const QUICK_COLORS = [
  "#000000",
  "#FFFFFF",
  "#FF0000",
  "#00FF00",
  "#0000FF",
  "#FFFF00",
  "#FF00FF",
  "#00FFFF",
  "#FFA500",
  "#800080",
  "#008000",
  "#808080"
] as const;

export const DrawTab = ({
  activeTool,
  setActiveTool,
  drawColor,
  setDrawColor,
  brushSize,
  setBrushSize
}: DrawTabProps) => (
  <div className="space-y-4 pt-4">
    <div>
      <Label className="bg-linear-to-r from-cyan-600 to-blue-600 bg-clip-text text-sm font-semibold text-transparent dark:from-cyan-400 dark:to-blue-400">
        Drawing Tools
      </Label>
      <div className="mt-3 grid grid-cols-2 gap-2">
        <Button
          variant={activeTool === "pencil" ? "default" : "outline"}
          size="sm"
          onClick={() => setActiveTool("pencil")}
        >
          <Pencil className="mr-2 size-4" />
          Pencil
        </Button>
        <Button
          variant={activeTool === "eraser" ? "default" : "outline"}
          size="sm"
          onClick={() => setActiveTool("eraser")}
        >
          <Eraser className="mr-2 size-4" />
          Eraser
        </Button>
        <Button
          variant={activeTool === "rectangle" ? "default" : "outline"}
          size="sm"
          onClick={() => setActiveTool("rectangle")}
        >
          <Square className="mr-2 size-4" />
          Rectangle
        </Button>
        <Button
          variant={activeTool === "circle" ? "default" : "outline"}
          size="sm"
          onClick={() => setActiveTool("circle")}
        >
          <Circle className="mr-2 size-4" />
          Circle
        </Button>
        <Button variant={activeTool === "line" ? "default" : "outline"} size="sm" onClick={() => setActiveTool("line")}>
          <Minus className="mr-2 size-4" />
          Line
        </Button>
        <Button variant={activeTool === "fill" ? "default" : "outline"} size="sm" onClick={() => setActiveTool("fill")}>
          <PaintBucket className="mr-2 size-4" />
          Fill
        </Button>
        <Button variant={activeTool === "crop" ? "default" : "outline"} size="sm" onClick={() => setActiveTool("crop")}>
          <Crop className="mr-2 size-4" />
          Crop
        </Button>
        <Button variant={activeTool === "none" ? "default" : "outline"} size="sm" onClick={() => setActiveTool("none")}>
          <Move className="mr-2 size-4" />
          None
        </Button>
      </div>
    </div>

    <Separator />

    <div className="space-y-3">
      <div className="space-y-2">
        <Label htmlFor="draw-color" className="text-sm">
          Drawing Color
        </Label>
        <div className="flex items-center gap-3">
          <Input
            id="draw-color"
            type="color"
            value={drawColor}
            onChange={(e) => setDrawColor(e.target.value)}
            className="h-10 w-20 cursor-pointer"
          />
          <Input
            type="text"
            value={drawColor}
            onChange={(e) => setDrawColor(e.target.value)}
            className="flex-1 font-mono text-sm"
            placeholder="#000000"
          />
        </div>
      </div>

      <div className="space-y-2">
        <div className="flex items-center justify-between">
          <Label htmlFor="brush-size" className="text-sm">
            Brush Size
          </Label>
          <span className="text-muted-foreground text-xs">{brushSize}px</span>
        </div>
        <Slider
          id="brush-size"
          min={1}
          max={50}
          step={1}
          value={[brushSize]}
          onValueChange={([value]) => setBrushSize(value)}
        />
      </div>
    </div>

    <Separator />

    <div className="space-y-2">
      <Label className="text-sm">Quick Colors</Label>
      <div className="grid grid-cols-6 gap-2">
        {QUICK_COLORS.map((color) => (
          <button
            key={color}
            onClick={() => setDrawColor(color)}
            className="h-8 w-full rounded border-2 transition-all hover:scale-110"
            style={{
              backgroundColor: color,
              borderColor: drawColor === color ? "#3b82f6" : "#d1d5db"
            }}
            title={color}
          />
        ))}
      </div>
    </div>

    {activeTool !== "none" && (
      <Alert>
        <AlertDescription className="text-xs">
          {activeTool === "crop" && "Click and drag on the canvas to select the area to crop."}
          {activeTool === "pencil" && "Click and drag on the canvas to draw."}
          {activeTool === "eraser" && "Click and drag on the canvas to erase."}
          {activeTool === "fill" && "Click on an area to fill it with the selected color."}
          {activeTool === "line" && "Click and drag to draw a straight line."}
          {(activeTool === "rectangle" || activeTool === "circle") && "Click and drag to draw shapes."}
        </AlertDescription>
      </Alert>
    )}
  </div>
);
