import { Binary, type LucideIcon, Network, Scan } from "lucide-react";

type Feature = {
  icon: LucideIcon;
  label: string;
};

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
