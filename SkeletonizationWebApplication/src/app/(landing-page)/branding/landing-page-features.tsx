import { Binary, type LucideIcon, Network, Scan } from "lucide-react";

type LandingFeature = {
  icon: LucideIcon;
  title: string;
  description: string;
};

export const landingPageFeatures: LandingFeature[] = [
  {
    icon: Binary,
    title: "Binary Thinning",
    description: "Zhang-Suen, Guo-Hall, and morphological thinning"
  },
  {
    icon: Scan,
    title: "Edge Detection",
    description: "Canny, Sobel, and adaptive preprocessing filters"
  },
  {
    icon: Network,
    title: "Topology Extraction",
    description: "Medial axis transform and centerline detection"
  }
];
