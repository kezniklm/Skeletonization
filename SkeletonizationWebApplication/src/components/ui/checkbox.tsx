/**
 * @file checkbox.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Styled checkbox primitive component.
 * @description Wraps Radix checkbox behavior with application-specific styling and icon indicator.
 * @version 1.0
 * @date 2026-03-16
 */

import * as CheckboxPrimitive from "@radix-ui/react-checkbox";
import { Check } from "lucide-react";
import { type ComponentPropsWithoutRef, type ComponentRef, forwardRef } from "react";

import { cn } from "@/lib/utils";

/**
 * @brief Checkbox input with consistent visual states.
 */
export const Checkbox = forwardRef<
  ComponentRef<typeof CheckboxPrimitive.Root>,
  ComponentPropsWithoutRef<typeof CheckboxPrimitive.Root>
>(({ className, ...props }, ref) => (
  <CheckboxPrimitive.Root
    ref={ref}
    className={cn(
      "peer h-4 w-4 shrink-0 rounded-sm border border-cyan-600 ring-offset-white focus-visible:ring-2 focus-visible:ring-cyan-600 focus-visible:ring-offset-2 focus-visible:outline-none disabled:cursor-not-allowed disabled:opacity-50 data-[state=checked]:bg-cyan-600 data-[state=checked]:text-white dark:border-cyan-400 dark:ring-offset-gray-950 dark:focus-visible:ring-cyan-400 dark:data-[state=checked]:bg-cyan-400 dark:data-[state=checked]:text-gray-900",
      className
    )}
    {...props}
  >
    <CheckboxPrimitive.Indicator className={cn("flex items-center justify-center text-current")}>
      <Check className="h-4 w-4" />
    </CheckboxPrimitive.Indicator>
  </CheckboxPrimitive.Root>
));
Checkbox.displayName = CheckboxPrimitive.Root.displayName;
