/**
 * @file navigation-logo.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Renders clickable navigation brand mark.
 * @description Wraps the shared logo icon in a styled home-link container.
 * @version 1.0
 * @date 2026-03-16
 */

import { Logo } from "../logo/logo";

import { NavigationLink } from "./navigation-link";

type NavigationLogoProps = {
  className?: string;
};

/**
 * @brief Displays the navigation logo linked to home.
 * @param className Optional wrapper classes.
 * @returns Navigation logo element.
 */
export const NavigationLogo = ({ className }: NavigationLogoProps) => (
  <div className={className}>
    <NavigationLink
      href="/"
      className="group flex h-10 w-10 items-center justify-center rounded-lg bg-linear-to-br from-cyan-600 to-blue-600 shadow-md transition-all hover:scale-105 hover:shadow-lg"
    >
      <Logo />
    </NavigationLink>
  </div>
);
