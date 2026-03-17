/**
 * @file empty-state.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Empty state card component with a primary action.
 * @description Renders a reusable empty-state surface with icon, text, and CTA for workflow entry points.
 * @version 1.0
 * @date 2026-03-16
 */

"use client";

import { type LucideIcon } from "lucide-react";

import { Button } from "@/components/ui/button";
import { Card, CardContent } from "@/components/ui/card";

type EmptyStateProps = {
  icon: LucideIcon;
  title: string;
  description: string;
  actionLabel: string;
  actionIcon: LucideIcon;
  onAction: () => void;
  helpText?: string;
  className?: string;
};

/**
 * @brief Displays an actionable empty-state card.
 */
export const EmptyState = ({
  icon: Icon,
  title,
  description,
  actionLabel,
  actionIcon: ActionIcon,
  onAction,
  helpText,
  className
}: EmptyStateProps) => (
  <Card
    className={`border-2 border-dashed border-cyan-300 bg-linear-to-br from-cyan-50/50 to-blue-50/50 shadow-sm backdrop-blur-sm transition-all duration-300 hover:border-cyan-400 hover:shadow-xl dark:border-cyan-700 dark:from-cyan-950/20 dark:to-blue-950/20 dark:hover:border-cyan-600 ${className ?? ""}`}
  >
    <CardContent className="flex h-full min-h-75 flex-col items-center justify-center space-y-4 py-12">
      <div className="rounded-full bg-linear-to-r from-cyan-500 to-blue-500 p-4">
        <Icon className="size-10 text-white" />
      </div>
      <div className="space-y-2 text-center">
        <h3 className="text-xl font-bold text-gray-900 dark:text-white">{title}</h3>
        <p className="max-w-md text-sm text-gray-600 dark:text-gray-400">{description}</p>
      </div>
      <div className="flex flex-col gap-3 sm:flex-row">
        <Button onClick={onAction}>
          <ActionIcon className="mr-2 size-5" />
          {actionLabel}
        </Button>
      </div>
      {helpText && <p className="text-xs text-gray-500 dark:text-gray-500">{helpText}</p>}
    </CardContent>
  </Card>
);
