"use client";

import { FlipHorizontal, FlipVertical, RotateCcw, RotateCw } from "lucide-react";
import { type Dispatch, type SetStateAction } from "react";

import { Button } from "@/components/ui/button";
import { Label } from "@/components/ui/label";
import { Slider } from "@/components/ui/slider";
import { Separator } from "@/components/ui/separator";

import { type TransformState } from "../../types";

type TransformTabProps = {
  transforms: TransformState;
  setTransforms: Dispatch<SetStateAction<TransformState>>;
  onResetTransforms: () => void;
  onAddHistory: (description: string) => void;
};

const normalizeRotation = (value: number) => ((value % 360) + 360) % 360;

export const TransformTab = ({ transforms, setTransforms, onResetTransforms, onAddHistory }: TransformTabProps) => {
  const rotateBy = (delta: number) => {
    const nextRotation = normalizeRotation(transforms.rotation + delta);
    setTransforms((prev) => ({ ...prev, rotation: nextRotation }));
    onAddHistory(`Rotated to ${nextRotation}°`);
  };

  return (
    <div className="space-y-4 pt-4">
      <div className="flex items-center justify-between">
        <Label className="bg-linear-to-r from-cyan-600 to-blue-600 bg-clip-text text-sm font-semibold text-transparent dark:from-cyan-400 dark:to-blue-400">
          Image Transformations
        </Label>
        <Button
          variant="ghost"
          size="sm"
          onClick={onResetTransforms}
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
            <Label htmlFor="rotation">Rotation</Label>
            <span className="text-muted-foreground text-xs">{transforms.rotation}°</span>
          </div>
          <Slider
            id="rotation"
            min={0}
            max={360}
            step={1}
            value={[transforms.rotation]}
            onValueChange={([value]) => setTransforms((prev) => ({ ...prev, rotation: value }))}
            onValueCommit={() => onAddHistory(`Rotated to ${transforms.rotation}°`)}
          />
        </div>

        <div className="space-y-2">
          <div className="flex items-center justify-between">
            <Label htmlFor="scale">Scale</Label>
            <span className="text-muted-foreground text-xs">{transforms.scale.toFixed(2)}x</span>
          </div>
          <Slider
            id="scale"
            min={0.1}
            max={3}
            step={0.1}
            value={[transforms.scale]}
            onValueChange={([value]) => setTransforms((prev) => ({ ...prev, scale: value }))}
            onValueCommit={() => onAddHistory(`Scaled to ${transforms.scale.toFixed(2)}x`)}
          />
        </div>

        <Separator />

        <div className="space-y-2">
          <Label className="text-gray-700 dark:text-gray-300">Flip Operations</Label>
          <div className="grid grid-cols-2 gap-2">
            <Button
              variant={transforms.flipH ? "default" : "outline"}
              size="sm"
              onClick={() => {
                setTransforms((prev) => ({ ...prev, flipH: !prev.flipH }));
                onAddHistory("Flipped horizontally");
              }}
            >
              <FlipHorizontal className="mr-2 size-4" />
              Horizontal
            </Button>
            <Button
              variant={transforms.flipV ? "default" : "outline"}
              size="sm"
              onClick={() => {
                setTransforms((prev) => ({ ...prev, flipV: !prev.flipV }));
                onAddHistory("Flipped vertically");
              }}
            >
              <FlipVertical className="mr-2 size-4" />
              Vertical
            </Button>
          </div>
        </div>

        <Separator />

        <div className="space-y-2">
          <Label className="text-gray-700 dark:text-gray-300">Quick Actions</Label>
          <div className="grid grid-cols-2 gap-2">
            <Button variant="outline" size="sm" onClick={() => rotateBy(90)}>
              <RotateCw className="mr-2 size-4" />
              +90°
            </Button>
            <Button variant="outline" size="sm" onClick={() => rotateBy(180)}>
              <RotateCw className="mr-2 size-4" />
              +180°
            </Button>
          </div>
        </div>
      </div>
    </div>
  );
};
