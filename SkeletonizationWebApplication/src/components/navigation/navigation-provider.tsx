/**
 * @file navigation-provider.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Provides navigation UI state context.
 * @description Stores and exposes mobile menu open state and toggle helpers for navigation components.
 * @version 1.0
 * @date 2026-03-16
 */

"use client";

import { createContext, useContext, useState, type ReactNode } from "react";

type NavigationContextType = {
  isMobileMenuOpen: boolean;
  setIsMobileMenuOpen: (value: boolean) => void;
  toggleMobileMenu: () => void;
};

const NavigationContext = createContext<NavigationContextType | undefined>(undefined);

/**
 * @brief Returns navigation context values.
 * @description Throws when accessed outside of `NavigationProvider`.
 * @returns Navigation context state and handlers.
 */
export const useNavigation = () => {
  const context = useContext(NavigationContext);
  if (!context) {
    throw new Error("useNavigation must be used within NavigationProvider");
  }
  return context;
};

type NavigationProviderProps = {
  children: ReactNode;
};

/**
 * @brief Wraps children with navigation state provider.
 * @description Supplies mobile menu state and toggle methods to nested navigation components.
 * @param children Descendant components requiring navigation context.
 * @returns Context provider element.
 */
export const NavigationProvider = ({ children }: NavigationProviderProps) => {
  const [isMobileMenuOpen, setIsMobileMenuOpen] = useState(false);

  const toggleMobileMenu = () => setIsMobileMenuOpen((prev) => !prev);

  return (
    <NavigationContext.Provider value={{ isMobileMenuOpen, setIsMobileMenuOpen, toggleMobileMenu }}>
      {children}
    </NavigationContext.Provider>
  );
};
