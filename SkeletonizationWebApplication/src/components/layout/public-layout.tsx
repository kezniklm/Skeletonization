/**
 * @file public-layout.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Defines layout shell for public pages.
 * @description Renders public navigation badges and page container for unauthenticated or landing routes.
 * @version 1.0
 * @date 2026-03-16
 */

import { navigationBadges } from "../navigation/constants/navigation-badges";
import { Navigation } from "../navigation/navigation";

/**
 * @brief Renders public application layout.
 * @description Filters visible navigation badges for public routes and renders main page content.
 * @param children Nested page content.
 * @returns Public layout wrapper with navigation.
 */
export const PublicLayout = ({
  children
}: Readonly<{
  children: React.ReactNode;
}>) => {
  const publicNavigationBadges = navigationBadges.filter(
    (badge) => badge.visibility === "public" || badge.visibility === "always"
  );

  return (
    <div className="flex min-h-screen flex-col">
      <Navigation navigationBadges={publicNavigationBadges} navigationItems={[]} />

      <main className="container mx-auto px-8 py-13 lg:max-w-4/5 2xl:py-16">{children}</main>
    </div>
  );
};
