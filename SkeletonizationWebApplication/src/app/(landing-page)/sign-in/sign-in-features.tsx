/**
 * @file sign-in-features.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Defines static feature bullets for sign-in panel.
 * @description Provides lightweight capability messages rendered below social sign-in actions.
 * @version 1.0
 * @date 2026-03-16
 */

import { Binary, type LucideIcon, Network, Scan } from "lucide-react";

/**
 * @brief Represents one sign-in feature line item.
 */
type Feature = {
  icon: LucideIcon;
  label: string;
};

/**
 * @brief Stores sign-in panel feature labels and icons.
 * @description Used to render the short benefits list on the landing authentication card.
 */
export const signInFeatures: Feature[] = [
  {
    icon: Binary,
    label: "Pixel-perfect accuracy"
  },
  {
    icon: Scan,
    label: "Real-time processing"
  },
  {
    icon: Network,
    label: "Topology preservation"
  }
];
