/**
 * @file label.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Form label component with accessibility styling.
 * @description Exposes a forwardRef label primitive used to annotate form controls consistently.
 * @version 1.0
 * @date 2026-03-16
 */

import { forwardRef, type LabelHTMLAttributes } from "react";

import { cn } from "@/lib/utils";

/**
 * @brief Renders a styled label element.
 */
const Label = forwardRef<HTMLLabelElement, LabelHTMLAttributes<HTMLLabelElement>>(({ className, ...props }, ref) => (
  // eslint-disable-next-line jsx-a11y/label-has-associated-control
  <label
    ref={ref}
    className={cn(
      "text-sm leading-none font-medium peer-disabled:cursor-not-allowed peer-disabled:opacity-70",
      className
    )}
    {...props}
  />
));
Label.displayName = "Label";

export { Label };
