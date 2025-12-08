import { Binary, Network, Scan, Image as ImageIcon } from "lucide-react";
import type { ReactElement } from "react";

export type NavigationItem = {
  name: string;
  href: string;
  icon: ReactElement;
  description?: string;
};

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
