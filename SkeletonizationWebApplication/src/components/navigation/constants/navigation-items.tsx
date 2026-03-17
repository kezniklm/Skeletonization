/**
 * @file navigation-items.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Defines primary authenticated navigation links.
 * @description Exports navigation item type and static route metadata used across desktop and mobile navigation.
 * @version 1.0
 * @date 2026-03-16
 */

import { Binary, Network, Scan, Image as ImageIcon } from "lucide-react";
import type { ReactElement } from "react";

/**
 * @brief Describes one navigation link entry.
 */
export type NavigationItem = {
  name: string;
  href: string;
  icon: ReactElement;
  description?: string;
};

/**
 * @brief Static navigation link definitions.
 * @description Contains route, icon, and helper text for main application sections.
 */
export const navigationItems: NavigationItem[] = [
  {
    name: "Images",
    href: "/images",
    icon: <ImageIcon className="h-4 w-4" />,
    description: "Image library"
  },
  {
    name: "Preprocessing",
    href: "/preprocessing",
    icon: <Scan className="h-4 w-4" />,
    description: "Filters & enhancement"
  },
  {
    name: "Skeletonization",
    href: "/skeletonization",
    icon: <Binary className="h-4 w-4" />,
    description: "Thinning algorithms"
  },
  {
    name: "Lab",
    href: "/lab",
    icon: <Network className="h-4 w-4" />,
    description: "Processing workspace"
  }
];
