/**
 * @file desktop-navigation.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Renders desktop navigation links.
 * @description Displays horizontal navigation entries with icon and hover affordances for larger screens.
 * @version 1.0
 * @date 2026-03-16
 */

import { type NavigationItem } from "./constants/navigation-items";
import { NavigationLink } from "./navigation-link";

type DesktopNavigationProps = {
  className: string;
  navigationItems: NavigationItem[];
};

/**
 * @brief Displays desktop-only navigation link list.
 * @description Maps configured navigation items into styled link entries.
 * @param className Class names applied to the list container.
 * @param navigationItems Navigation item definitions.
 * @returns Desktop navigation list element.
 */
export const DesktopNavigation = ({ navigationItems, className }: DesktopNavigationProps) => (
  <ul className={className}>
    {navigationItems.map((link) => (
      <li key={link.name}>
        <NavigationLink
          href={link.href}
          className="group relative flex items-center space-x-2 rounded-lg px-4 py-2 text-sm font-medium text-gray-700 transition-all hover:bg-cyan-50 hover:text-cyan-700 dark:text-gray-300 dark:hover:bg-cyan-950/30 dark:hover:text-cyan-400"
        >
          <span className="text-cyan-600 dark:text-cyan-500">{link.icon}</span>
          <span>{link.name}</span>
          <span className="absolute right-4 bottom-0 left-4 h-0.5 scale-x-0 bg-linear-to-r from-cyan-600 to-blue-600 transition-transform group-hover:scale-x-100" />
        </NavigationLink>
      </li>
    ))}
  </ul>
);
