import { type NavigationItem } from "./constants/navigation-items";
import { NavigationLink } from "./navigation-link";

type DesktopNavigationProps = {
  className: string;
  navigationItems: NavigationItem[];
};

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
          <span className="absolute right-4 bottom-0 left-4 h-0.5 scale-x-0 bg-gradient-to-r from-cyan-600 to-blue-600 transition-transform group-hover:scale-x-100" />
        </NavigationLink>
      </li>
    ))}
  </ul>
);
