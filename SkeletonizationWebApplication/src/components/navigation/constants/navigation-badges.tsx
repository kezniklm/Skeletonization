import { Cpu, Cloud, Layers, Binary } from "lucide-react";
import { type ReactElement } from "react";

import { algorithms } from "@/algorithms";

export type NavigationBadge = {
  icon: ReactElement;
  label: string;
  visibility: "always" | "public" | "private";
};

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
    label: "Auto-Scale",
    visibility: "public"
  },
  {
    icon: <Layers className="h-3.5 w-3.5 text-cyan-600 dark:text-cyan-400" />,
    label: `${algorithms.length} Algorithms`,
    visibility: "always"
  }
];
