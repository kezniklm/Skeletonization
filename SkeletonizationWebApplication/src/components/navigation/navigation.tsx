/**
 * @file navigation.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Renders top-level application navigation shell.
 * @description Composes desktop and mobile navigation, badge display, and user menu within provider-managed state.
 * @version 1.0
 * @date 2026-03-16
 */

import { DesktopNavigation } from "./desktop-navigation";
import { type NavigationBadge } from "./constants/navigation-badges";
import { type NavigationItem } from "./constants/navigation-items";
import { NavigationLogo } from "./navigation-logo";
import { NavigationBadges } from "./navigation-badges";
import NavigationUserMenu from "./navigation-user-menu";
import { MobileNavigationButton, MobileNavigationMenu } from "./mobile-navigation";
import { NavigationProvider } from "./navigation-provider";

type NavigationProps = {
  navigationItems: NavigationItem[];
  navigationBadges: NavigationBadge[];
};

/**
 * @brief Displays responsive global navigation.
 * @description Renders desktop links, mobile menu, badges, and account controls using provided navigation metadata.
 * @param navigationItems Primary navigation links.
 * @param navigationBadges Header badges to display.
 * @returns Sticky navigation header component.
 */
export const Navigation = ({ navigationItems, navigationBadges }: NavigationProps) => (
  <NavigationProvider>
    <nav className="sticky top-0 z-50 border-b border-gray-200/50 bg-white/90 backdrop-blur-lg dark:border-gray-800/50 dark:bg-gray-950/90">
      <div className="mx-auto max-w-7xl px-4 sm:px-6 lg:px-8">
        <div className="flex h-16 items-center justify-between">
          <NavigationLogo className="flex items-center" />

          {navigationItems.length > 0 && (
            <DesktopNavigation className="hidden items-center space-x-1 md:flex" navigationItems={navigationItems} />
          )}

          <div className="flex items-center space-x-4">
            {navigationBadges.length > 0 && <NavigationBadges navigationBadges={navigationBadges} />}

            <NavigationUserMenu className="relative" />

            {navigationItems.length > 0 && <MobileNavigationButton />}
          </div>
        </div>

        {navigationItems.length > 0 && <MobileNavigationMenu navigationItems={navigationItems} />}
      </div>
    </nav>
  </NavigationProvider>
);
