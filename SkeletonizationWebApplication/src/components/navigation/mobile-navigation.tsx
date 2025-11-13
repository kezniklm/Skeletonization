"use client";

import { Menu, X } from "lucide-react";

import { authClient } from "@/auth-client";

import { type NavigationItem } from "./constants/navigation-items";
import { useNavigation } from "./navigation-provider";
import { NavigationLink } from "./navigation-link";

export const MobileNavigationButton = () => {
  const { isMobileMenuOpen, toggleMobileMenu } = useNavigation();

  return (
    <button
      onClick={toggleMobileMenu}
      className="rounded-lg p-2 text-cyan-700 hover:bg-cyan-50 md:hidden dark:text-cyan-400 dark:hover:bg-cyan-950/30"
    >
      {isMobileMenuOpen ? <X className="h-6 w-6" /> : <Menu className="h-6 w-6" />}
    </button>
  );
};

type MobileNavigationMenuProps = {
  navigationItems: NavigationItem[];
};

export const MobileNavigationMenu = ({ navigationItems }: MobileNavigationMenuProps) => {
  const { data: session } = authClient.useSession();
  const { isMobileMenuOpen, setIsMobileMenuOpen } = useNavigation();

  if (!isMobileMenuOpen || !session) return null;

  return (
    <div className="border-t border-gray-200/50 bg-gradient-to-b from-cyan-50/30 to-transparent py-4 md:hidden dark:border-gray-800/50 dark:from-cyan-950/10">
      <div className="space-y-1">
        {navigationItems.map((link) => (
          <NavigationLink
            key={link.name}
            href={link.href}
            className="group flex items-center justify-between rounded-lg px-4 py-3 text-sm font-medium text-gray-700 transition-colors hover:bg-cyan-50 hover:text-cyan-700 dark:text-gray-300 dark:hover:bg-cyan-950/30 dark:hover:text-cyan-400"
            onClick={() => setIsMobileMenuOpen(false)}
          >
            <div className="flex items-center space-x-3">
              <span className="text-cyan-600 dark:text-cyan-500">{link.icon}</span>
              <div>
                <div>{link.name}</div>
                <div className="text-xs text-gray-500 dark:text-gray-400">{link.description}</div>
              </div>
            </div>
          </NavigationLink>
        ))}
      </div>
    </div>
  );
};
