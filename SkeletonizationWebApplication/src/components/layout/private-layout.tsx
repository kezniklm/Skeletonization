/**
 * @file private-layout.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Defines layout shell for authenticated pages.
 * @description Renders private navigation and page container for routes requiring signed-in access.
 * @version 1.0
 * @date 2026-03-16
 */

import { navigationBadges } from "../navigation/constants/navigation-badges";
import { navigationItems } from "../navigation/constants/navigation-items";
import { Navigation } from "../navigation/navigation";

/**
 * @brief Renders authenticated application layout.
 * @description Filters visible navigation badges for private routes and renders main page content.
 * @param children Nested page content.
 * @returns Private layout wrapper with navigation.
 */
export const PrivateLayout = ({
  children
}: Readonly<{
  children: React.ReactNode;
}>) => {
  const privateNavigationBadges = navigationBadges.filter(
    (badge) => badge.visibility === "private" || badge.visibility === "always"
  );

  return (
    <div className="flex min-h-screen flex-col">
      <Navigation navigationBadges={privateNavigationBadges} navigationItems={navigationItems} />
      <main className="container mx-auto px-8 py-13 lg:max-w-4/5 2xl:py-16">{children}</main>
    </div>
  );
};
