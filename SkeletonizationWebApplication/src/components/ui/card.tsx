/**
 * @file card.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Card layout primitives for structured content sections.
 * @description Defines composable card subcomponents used to build consistent container surfaces across the app.
 * @version 1.0
 * @date 2026-03-16
 */

import { cn } from "@/lib/utils";

/**
 * @brief Card root container component.
 */
const Card = ({ className, ...props }: React.ComponentProps<"div">) => (
  <div
    data-slot="card"
    className={cn("bg-card text-card-foreground flex flex-col gap-6 rounded-xl border py-6 shadow-sm", className)}
    {...props}
  />
);

/**
 * @brief Card header region component.
 */
const CardHeader = ({ className, ...props }: React.ComponentProps<"div">) => (
  <div
    data-slot="card-header"
    className={cn(
      "@container/card-header grid auto-rows-min grid-rows-[auto_auto] items-start gap-2 px-6 has-data-[slot=card-action]:grid-cols-[1fr_auto] [.border-b]:pb-6",
      className
    )}
    {...props}
  />
);

/**
 * @brief Card title text component.
 */
const CardTitle = ({ className, ...props }: React.ComponentProps<"div">) => (
  <div data-slot="card-title" className={cn("leading-none font-semibold", className)} {...props} />
);

/**
 * @brief Card descriptive text component.
 */
const CardDescription = ({ className, ...props }: React.ComponentProps<"div">) => (
  <div data-slot="card-description" className={cn("text-muted-foreground text-sm", className)} {...props} />
);

/**
 * @brief Card action slot component.
 */
const CardAction = ({ className, ...props }: React.ComponentProps<"div">) => (
  <div
    data-slot="card-action"
    className={cn("col-start-2 row-span-2 row-start-1 self-start justify-self-end", className)}
    {...props}
  />
);

/**
 * @brief Card body content component.
 */
const CardContent = ({ className, ...props }: React.ComponentProps<"div">) => (
  <div data-slot="card-content" className={cn("px-6", className)} {...props} />
);

/**
 * @brief Card footer region component.
 */
const CardFooter = ({ className, ...props }: React.ComponentProps<"div">) => (
  <div data-slot="card-footer" className={cn("flex items-center px-6 [.border-t]:pt-6", className)} {...props} />
);

export { Card, CardHeader, CardFooter, CardTitle, CardAction, CardDescription, CardContent };
