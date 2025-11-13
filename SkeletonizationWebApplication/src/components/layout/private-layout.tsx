import { navigationBadges } from "../navigation/constants/navigation-badges";
import { navigationItems } from "../navigation/constants/navigation-items";
import { Navigation } from "../navigation/navigation";

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
      <main className="container mx-auto px-8 py-8 lg:max-w-4/5">{children}</main>
    </div>
  );
};
