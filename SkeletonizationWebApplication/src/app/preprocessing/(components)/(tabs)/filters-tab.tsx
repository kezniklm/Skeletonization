"use client";

import { RotateCcw, Settings2, Sparkles } from "lucide-react";
import { type Dispatch, type SetStateAction } from "react";

import { Button } from "@/components/ui/button";
import { Label } from "@/components/ui/label";
import { Slider } from "@/components/ui/slider";
import { Separator } from "@/components/ui/separator";

import { type FilterState } from "../../types";

type FiltersTabProps = {
  filters: FilterState;
  setFilters: Dispatch<SetStateAction<FilterState>>;
  onResetFilters: () => void;
  onAddHistory: (description: string) => void;
};

export const FiltersTab = ({ filters, setFilters, onResetFilters, onAddHistory }: FiltersTabProps) => (
  <div className="space-y-4 pt-4">
    <div className="flex items-center justify-between">
      <Label className="bg-linear-to-r from-cyan-600 to-blue-600 bg-clip-text text-sm font-semibold text-transparent dark:from-cyan-400 dark:to-blue-400">
        Predefined Filters
      </Label>
      <Button
        variant="ghost"
        size="sm"
        onClick={onResetFilters}
        className="text-cyan-600 hover:text-cyan-700 dark:text-cyan-400 dark:hover:text-cyan-300"
      >
        <RotateCcw className="size-3" />
        <span className="ml-1.5 text-xs">Reset</span>
      </Button>
    </div>

    <Separator />

    <div className="space-y-4">
      <div className="space-y-2">
        <div className="flex items-center justify-between">
          <Label htmlFor="blur">Blur</Label>
          <span className="text-muted-foreground text-xs">{filters.blur}</span>
        </div>
        <Slider
          id="blur"
          min={0}
          max={10}
          step={1}
          value={[filters.blur]}
          onValueChange={([value]) => setFilters((prev) => ({ ...prev, blur: value }))}
          onValueCommit={() => onAddHistory(`Adjusted blur to ${filters.blur}`)}
        />
      </div>

      <div className="space-y-2">
        <div className="flex items-center justify-between">
          <Label htmlFor="sharpen">Sharpen</Label>
          <span className="text-muted-foreground text-xs">{filters.sharpen}</span>
        </div>
        <Slider
          id="sharpen"
          min={0}
          max={5}
          step={1}
          value={[filters.sharpen]}
          onValueChange={([value]) => setFilters((prev) => ({ ...prev, sharpen: value }))}
          onValueCommit={() => onAddHistory(`Adjusted sharpen to ${filters.sharpen}`)}
        />
      </div>

      <div className="space-y-2">
        <div className="flex items-center justify-between">
          <Label htmlFor="brightness">Brightness</Label>
          <span className="text-muted-foreground text-xs">{filters.brightness}</span>
        </div>
        <Slider
          id="brightness"
          min={-100}
          max={100}
          step={1}
          value={[filters.brightness]}
          onValueChange={([value]) => setFilters((prev) => ({ ...prev, brightness: value }))}
          onValueCommit={() => onAddHistory(`Adjusted brightness to ${filters.brightness}`)}
        />
      </div>

      <div className="space-y-2">
        <div className="flex items-center justify-between">
          <Label htmlFor="contrast">Contrast</Label>
          <span className="text-muted-foreground text-xs">{filters.contrast.toFixed(2)}</span>
        </div>
        <Slider
          id="contrast"
          min={0.5}
          max={3}
          step={0.1}
          value={[filters.contrast]}
          onValueChange={([value]) => setFilters((prev) => ({ ...prev, contrast: value }))}
          onValueCommit={() => onAddHistory(`Adjusted contrast to ${filters.contrast.toFixed(2)}`)}
        />
      </div>

      {!filters.grayscale && (
        <>
          <div className="space-y-2">
            <div className="flex items-center justify-between">
              <Label htmlFor="saturation">Saturation</Label>
              <span className="text-muted-foreground text-xs">{filters.saturation.toFixed(2)}</span>
            </div>
            <Slider
              id="saturation"
              min={0}
              max={2}
              step={0.1}
              value={[filters.saturation]}
              onValueChange={([value]) => setFilters((prev) => ({ ...prev, saturation: value }))}
              onValueCommit={() => onAddHistory(`Adjusted saturation to ${filters.saturation.toFixed(2)}`)}
            />
          </div>

          <div className="space-y-2">
            <div className="flex items-center justify-between">
              <Label htmlFor="hue">Hue</Label>
              <span className="text-muted-foreground text-xs">{filters.hue}</span>
            </div>
            <Slider
              id="hue"
              min={-180}
              max={180}
              step={1}
              value={[filters.hue]}
              onValueChange={([value]) => setFilters((prev) => ({ ...prev, hue: value }))}
              onValueCommit={() => onAddHistory(`Adjusted hue to ${filters.hue}`)}
            />
          </div>
        </>
      )}

      <div className="space-y-2">
        <div className="flex items-center justify-between">
          <Label htmlFor="denoise">Denoise</Label>
          <span className="text-muted-foreground text-xs">{filters.denoise}</span>
        </div>
        <Slider
          id="denoise"
          min={0}
          max={20}
          step={1}
          value={[filters.denoise]}
          onValueChange={([value]) => setFilters((prev) => ({ ...prev, denoise: value }))}
          onValueCommit={() => onAddHistory(`Adjusted denoise to ${filters.denoise}`)}
        />
      </div>

      <Separator />

      <div className="space-y-2">
        <Label>Special Filters</Label>
        <div className="flex flex-col gap-2">
          <Button
            variant={filters.thresholding ? "default" : "outline"}
            size="sm"
            onClick={() => {
              setFilters((prev) => ({ ...prev, thresholding: !prev.thresholding }));
              onAddHistory(`Toggled thresholding ${!filters.thresholding ? "on" : "off"}`);
            }}
            className="justify-start"
          >
            <Settings2 className="mr-2 size-4" />
            Thresholding
          </Button>
          <Button
            variant={filters.grayscale ? "default" : "outline"}
            size="sm"
            onClick={() => {
              setFilters((prev) => ({ ...prev, grayscale: !prev.grayscale }));
              onAddHistory(`Toggled grayscale ${!filters.grayscale ? "on" : "off"}`);
            }}
            className="justify-start"
          >
            <Settings2 className="mr-2 size-4" />
            Grayscale
          </Button>
          <Button
            variant={filters.edgeDetection ? "default" : "outline"}
            size="sm"
            onClick={() => {
              setFilters((prev) => ({ ...prev, edgeDetection: !prev.edgeDetection }));
              onAddHistory(`Toggled Canny edge detection ${!filters.edgeDetection ? "on" : "off"}`);
            }}
            className="justify-start"
          >
            <Sparkles className="mr-2 size-4" />
            Canny Edge Detection
          </Button>
          <Button
            variant={filters.sobelEdgeDetection ? "default" : "outline"}
            size="sm"
            onClick={() => {
              setFilters((prev) => ({ ...prev, sobelEdgeDetection: !prev.sobelEdgeDetection }));
              onAddHistory(`Toggled Sobel edge detection ${!filters.sobelEdgeDetection ? "on" : "off"}`);
            }}
            className="justify-start"
          >
            <Sparkles className="mr-2 size-4" />
            Sobel Edge Detection
          </Button>
          <Button
            variant={filters.adaptiveThreshold ? "default" : "outline"}
            size="sm"
            onClick={() => {
              setFilters((prev) => ({ ...prev, adaptiveThreshold: !prev.adaptiveThreshold }));
              onAddHistory(`Toggled adaptive threshold ${!filters.adaptiveThreshold ? "on" : "off"}`);
            }}
            className="justify-start"
          >
            <Settings2 className="mr-2 size-4" />
            Adaptive Threshold
          </Button>
          <Button
            variant={filters.opening ? "default" : "outline"}
            size="sm"
            onClick={() => {
              setFilters((prev) => ({ ...prev, opening: !prev.opening }));
              onAddHistory(`Toggled opening ${!filters.opening ? "on" : "off"}`);
            }}
            className="justify-start"
          >
            <Sparkles className="mr-2 size-4" />
            Opening
          </Button>
          <Button
            variant={filters.closing ? "default" : "outline"}
            size="sm"
            onClick={() => {
              setFilters((prev) => ({ ...prev, closing: !prev.closing }));
              onAddHistory(`Toggled closing ${!filters.closing ? "on" : "off"}`);
            }}
            className="justify-start"
          >
            <Sparkles className="mr-2 size-4" />
            Closing
          </Button>
        </div>
      </div>

      {filters.thresholding && (
        <div className="space-y-2">
          <div className="flex items-center justify-between">
            <Label htmlFor="threshold">Threshold</Label>
            <span className="text-muted-foreground text-xs">
              {filters.threshold === 0 ? "Automatic" : filters.threshold}
            </span>
          </div>
          <Slider
            id="threshold"
            min={0}
            max={255}
            step={1}
            value={[filters.threshold]}
            onValueChange={([value]) => setFilters((prev) => ({ ...prev, threshold: value }))}
            onValueCommit={() =>
              onAddHistory(
                filters.threshold === 0 ? "Set threshold to Otsu auto" : `Adjusted threshold to ${filters.threshold}`
              )
            }
          />
        </div>
      )}
    </div>
  </div>
);
