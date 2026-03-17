/**
 * @file logo.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Renders brand logo icon.
 * @description Exposes a reusable icon-based logo component for navigation and branding surfaces.
 * @version 1.0
 * @date 2026-03-16
 */

import { GitBranch } from "lucide-react";

type LogoProps = {
  className?: string;
};

/**
 * @brief Displays the application logo icon.
 * @description Applies optional class overrides to the base logo styling.
 * @param className Optional additional classes.
 * @returns Logo icon element.
 */
export const Logo = ({ className }: LogoProps) => <GitBranch className={`h-6 w-6 text-white ${className}`} />;
