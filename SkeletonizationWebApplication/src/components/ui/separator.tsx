/**
 * @file separator.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Visual separator component for layout grouping.
 * @description Provides horizontal or vertical divider lines based on Radix separator primitives.
 * @version 1.0
 * @date 2026-03-16
 */

"use client";

import * as SeparatorPrimitive from "@radix-ui/react-separator";

import { cn } from "@/lib/utils";

/**
 * @brief Renders a horizontal or vertical separator.
 */
const Separator = ({
  className,
  orientation = "horizontal",
  decorative = true,
  ...props
}: React.ComponentProps<typeof SeparatorPrimitive.Root>) => (
  <SeparatorPrimitive.Root
    data-slot="separator"
    decorative={decorative}
    orientation={orientation}
    className={cn(
      "bg-border shrink-0 data-[orientation=horizontal]:h-px data-[orientation=horizontal]:w-full data-[orientation=vertical]:h-full data-[orientation=vertical]:w-px",
      className
    )}
    {...props}
  />
);

export { Separator };
