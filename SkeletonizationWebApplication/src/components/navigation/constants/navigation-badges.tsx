/**
 * @file navigation-badges.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Defines static navigation badge metadata.
 * @description Exports badge type and configured badges used in navigation header for status and capability cues.
 * @version 1.0
 * @date 2026-03-16
 */

import { Cpu, Cloud, Layers, Binary } from "lucide-react";
import { type ReactElement } from "react";

import { ALGORITHMS } from "@/algorithms";

/**
 * @brief Describes one navigation badge item.
 */
export type NavigationBadge = {
  icon: ReactElement;
  label: string;
  visibility: "always" | "public" | "private";
};

/**
 * @brief Static list of navigation badges.
 * @description Contains icon, text, and visibility settings for header badge rendering.
 */
export const navigationBadges: NavigationBadge[] = [
  {
    icon: <Binary className="h-3.5 w-3.5 text-cyan-600 dark:text-cyan-400" />,
    label: "Open Source",
    visibility: "public"
  },
  {
    icon: <Cpu className="h-3.5 w-3.5 text-cyan-600 dark:text-cyan-400" />,
    label: "GPU Parallel",
    visibility: "always"
  },
  {
    icon: <Cloud className="h-3.5 w-3.5 text-cyan-600 dark:text-cyan-400" />,
    label: "Cloud Native",
    visibility: "public"
  },
  {
    icon: <Layers className="h-3.5 w-3.5 text-cyan-600 dark:text-cyan-400" />,
    label: `${ALGORITHMS.length} Algorithms`,
    visibility: "always"
  }
];
