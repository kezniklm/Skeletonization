/**
 * @file landing-page-features.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Defines static landing-page feature descriptors.
 * @description Provides icon and text metadata used to render the product capabilities list.
 * @version 1.0
 * @date 2026-03-16
 */

import { Binary, type LucideIcon, Network, Scan } from "lucide-react";

/**
 * @brief Represents one landing-page feature descriptor.
 */
type LandingFeature = {
  icon: LucideIcon;
  title: string;
  description: string;
};

/**
 * @brief Stores feature entries shown in landing-page highlights.
 * @description Each item maps capability text to an icon used by the feature component.
 */
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
