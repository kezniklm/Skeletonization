/**
 * @file navigation-user-menu.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Renders authenticated user dropdown menu.
 * @description Provides account quick actions, profile navigation, and sign-out handling from header avatar menu.
 * @version 1.0
 * @date 2026-03-16
 */

"use client";

import { useEffect, useRef, useState } from "react";
import { useRouter } from "next/navigation";
import { ChevronDown, LogOut, Settings, User as UserIcon } from "lucide-react";

import { authClient } from "@/auth-client";

import NavigationAvatar from "./navigation-avatar";
import { NavigationLink } from "./navigation-link";

type UserMenuProps = {
  className?: string;
};

/**
 * @brief Displays user account menu in navigation header.
 * @description Manages dropdown open state, outside-click dismissal, and account action navigation.
 * @param className Optional wrapper classes.
 * @returns User menu dropdown or `null` when session is missing.
 */
const NavigationUserMenu = ({ className }: UserMenuProps) => {
  const { data: session } = authClient.useSession();
  const [isOpen, setIsOpen] = useState(false);
  const router = useRouter();
  const ref = useRef<HTMLDivElement | null>(null);

  useEffect(() => {
    const onDocClick = (e: MouseEvent) => {
      if (isOpen && ref.current && !ref.current.contains(e.target as Node)) {
        setIsOpen(false);
      }
    };
    const onKey = (e: KeyboardEvent) => {
      if (e.key === "Escape") setIsOpen(false);
    };
    document.addEventListener("mousedown", onDocClick);
    document.addEventListener("keydown", onKey);
    return () => {
      document.removeEventListener("mousedown", onDocClick);
      document.removeEventListener("keydown", onKey);
    };
  }, [isOpen]);

  const handleSignOut = async () => {
    await authClient.signOut();
    router.push("/");
  };

  if (!session) {
    return null;
  }

  return (
    <div className={className} ref={ref}>
      <button
        aria-haspopup="true"
        aria-expanded={isOpen}
        onClick={() => setIsOpen((s) => !s)}
        className="flex items-center space-x-3 rounded-full bg-linear-to-r from-cyan-50 to-blue-50 py-2 pr-3 pl-2 transition-all hover:from-cyan-100 hover:to-blue-100 hover:shadow-md dark:from-cyan-950/30 dark:to-blue-950/30 dark:hover:from-cyan-900/40 dark:hover:to-blue-900/40"
      >
        <div className="relative h-8 w-8 overflow-hidden rounded-full ring-2 ring-cyan-500 ring-offset-2 dark:ring-cyan-400">
          <NavigationAvatar src={session.user?.image ?? undefined} alt={session.user?.name ?? "User"} />
        </div>

        <span className="hidden text-sm font-medium text-gray-700 sm:inline-block dark:text-gray-300">
          {session.user?.name ?? "User"}
        </span>

        <ChevronDown
          className={`h-4 w-4 text-cyan-600 transition-transform dark:text-cyan-400 ${isOpen ? "rotate-180" : ""}`}
        />
      </button>

      {isOpen && (
        <>
          <div className="fixed inset-0 z-10" aria-hidden onClick={() => setIsOpen(false)} />
          <div
            role="menu"
            aria-orientation="vertical"
            aria-label="User menu"
            className="absolute right-0 z-20 mt-2 w-56 origin-top-right rounded-xl border border-gray-200/50 bg-white/95 shadow-xl backdrop-blur-lg dark:border-gray-700/50 dark:bg-gray-900/95"
          >
            <div className="border-b border-gray-200/50 bg-linear-to-r from-cyan-50/50 to-blue-50/50 px-4 py-3 dark:border-gray-700/50 dark:from-cyan-950/20 dark:to-blue-950/20">
              <p className="text-sm font-medium text-gray-900 dark:text-white">{session.user?.name}</p>
              <p className="truncate text-xs text-gray-500 dark:text-gray-400">{session.user?.email}</p>
            </div>
            <div className="py-2">
              <NavigationLink
                href="/profile"
                className="flex items-center space-x-2 px-4 py-2 text-sm text-gray-700 transition-colors hover:bg-cyan-50 hover:text-cyan-700 dark:text-gray-300 dark:hover:bg-cyan-950/30 dark:hover:text-cyan-400"
                onClick={() => setIsOpen(false)}
              >
                <UserIcon className="h-4 w-4" />
                <span>Profile</span>
              </NavigationLink>
              <NavigationLink
                href="/settings"
                className="flex items-center space-x-2 px-4 py-2 text-sm text-gray-700 transition-colors hover:bg-cyan-50 hover:text-cyan-700 dark:text-gray-300 dark:hover:bg-cyan-950/30 dark:hover:text-cyan-400"
                onClick={() => setIsOpen(false)}
              >
                <Settings className="h-4 w-4" />
                <span>Settings</span>
              </NavigationLink>
            </div>
            <div className="border-t border-gray-200/50 py-2 dark:border-gray-700/50">
              <button
                onClick={handleSignOut}
                className="flex w-full items-center space-x-2 px-4 py-2 text-sm text-red-600 transition-colors hover:bg-red-50 dark:text-red-400 dark:hover:bg-red-900/20"
              >
                <LogOut className="h-4 w-4" />
                <span>Sign out</span>
              </button>
            </div>
          </div>
        </>
      )}
    </div>
  );
};

export default NavigationUserMenu;
