import { navigationBadges } from "../navigation/constants/navigation-badges";
import { Navigation } from "../navigation/navigation";

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

      <main className="container mx-auto px-8 py-16 lg:max-w-4/5">{children}</main>
    </div>
  );
};
