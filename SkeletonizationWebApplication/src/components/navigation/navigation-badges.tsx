import { type NavigationBadge } from "./constants/navigation-badges";

type NavigationBadgesProps = {
  navigationBadges: NavigationBadge[];
};

export const NavigationBadges = ({ navigationBadges }: NavigationBadgesProps) => (
  <div className="hidden items-center space-x-2 lg:flex">
    {navigationBadges.map((badge, idx) => (
      <div
        key={idx}
        className="flex items-center space-x-1.5 rounded-lg bg-gradient-to-r from-cyan-50 to-blue-50 px-3 py-1.5 dark:from-cyan-950/30 dark:to-blue-950/30"
      >
        {badge.icon}
        <span className="text-xs font-medium text-gray-700 dark:text-gray-300">{badge.label}</span>
      </div>
    ))}
  </div>
);
