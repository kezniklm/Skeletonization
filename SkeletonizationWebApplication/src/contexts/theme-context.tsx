/**
 * @file theme-context.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Manages application theme selection and resolution.
 * @description Provides theme context with system-theme subscription and document class synchronization.
 * @version 1.0
 * @date 2026-03-16
 */

"use client";

import { createContext, useContext, useEffect, useState, useSyncExternalStore, type PropsWithChildren } from "react";

type Theme = "light" | "dark" | "system";
type ResolvedTheme = "light" | "dark";

type ThemeContextType = {
  theme: Theme;
  resolvedTheme: ResolvedTheme;
  setTheme: (theme: Theme) => void;
};

const ThemeContext = createContext<ThemeContextType | undefined>(undefined);

const getSystemTheme = (): ResolvedTheme => {
  if (typeof window === "undefined") {
    return "dark";
  }
  return window.matchMedia("(prefers-color-scheme: dark)").matches ? "dark" : "light";
};

const subscribeSystemTheme = (callback: () => void) => {
  if (typeof window === "undefined") {
    return () => {};
  }

  const mediaQuery = window.matchMedia("(prefers-color-scheme: dark)");
  const handler = () => callback();

  mediaQuery.addEventListener("change", handler);
  return () => mediaQuery.removeEventListener("change", handler);
};

const getSystemThemeSnapshot = () => getSystemTheme();
const getSystemThemeServerSnapshot = () => "dark" as ResolvedTheme;

type ThemeProviderProps = PropsWithChildren<{
  initialTheme?: Theme;
}>;

/**
 * @brief Provides theme context and resolved theme value.
 * @description Maintains explicit or system-derived theme and updates root `dark` class accordingly.
 * @param children Descendant nodes requiring theme context.
 * @param initialTheme Initial theme preference.
 * @returns Theme context provider wrapper.
 */
export const ThemeProvider = ({ children, initialTheme = "system" }: ThemeProviderProps) => {
  const [theme, setThemeState] = useState<Theme>(initialTheme);

  const systemTheme = useSyncExternalStore(subscribeSystemTheme, getSystemThemeSnapshot, getSystemThemeServerSnapshot);

  const resolvedTheme: ResolvedTheme = theme === "system" ? systemTheme : theme === "dark" ? "dark" : "light";

  const setTheme = (newTheme: Theme) => {
    setThemeState(newTheme);
  };

  useEffect(() => {
    const root = document.documentElement;

    if (resolvedTheme === "dark") {
      root.classList.add("dark");
    } else {
      root.classList.remove("dark");
    }
  }, [resolvedTheme]);

  return <ThemeContext.Provider value={{ theme, resolvedTheme, setTheme }}>{children}</ThemeContext.Provider>;
};

/**
 * @brief Returns current theme context values.
 * @description Throws when called outside `ThemeProvider`.
 * @returns Theme context including selected and resolved values.
 */
export const useTheme = () => {
  const context = useContext(ThemeContext);

  if (!context) {
    throw new Error("useTheme must be used within a ThemeProvider");
  }

  return context;
};
