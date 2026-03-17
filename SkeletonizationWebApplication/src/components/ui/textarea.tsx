/**
 * @file textarea.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Multi-line text input component.
 * @description Provides a consistent textarea primitive for longer user text entry.
 * @version 1.0
 * @date 2026-03-16
 */

import { type ComponentProps } from "react";

import { cn } from "@/lib/utils";

/**
 * @brief Renders a styled multiline textarea.
 */
const Textarea = ({ className, ...props }: ComponentProps<"textarea">) => (
  <textarea
    className={cn(
      "border-input bg-background ring-offset-background placeholder:text-muted-foreground focus-visible:ring-ring flex min-h-20 w-full rounded-md border px-3 py-2 text-sm focus-visible:ring-2 focus-visible:ring-offset-2 focus-visible:outline-none disabled:cursor-not-allowed disabled:opacity-50",
      className
    )}
    {...props}
  />
);

export { Textarea };
