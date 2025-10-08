import { createContext, useContext, useState, useEffect, useMemo, useCallback } from "react";
import type { ReactNode } from "react";

import type { Theme } from "../types";

type ThemeContextType = {
  theme: Theme;
  toggleTheme: () => void;
  getThemeClasses: () => {
    bg: string;
    bgSecondary: string;
    bgTertiary: string;
    text: string;
    textSecondary: string;
    textMuted: string;
    border: string;
    borderMuted: string;
    accent: string;
  };
};

const STORAGE_KEY = "skeletonization-theme";

const isBrowser = typeof window !== "undefined";

const applyDomTheme = (theme: Theme) => {
  if (!isBrowser) {
    return;
  }

  const root = document.documentElement;
  const body = document.body;

  root.classList.toggle("dark", theme === "dark");
  root.setAttribute("data-theme", theme);

  body.classList.remove("light", "dark");
  body.classList.add(theme);
};

const getInitialTheme = (): Theme => {
  if (!isBrowser) {
    return "dark";
  }

  const saved = localStorage.getItem(STORAGE_KEY) as Theme | null;

  if (saved === "light" || saved === "dark") {
    return saved;
  }

  const prefersDark = window.matchMedia?.("(prefers-color-scheme: dark)")?.matches;

  return prefersDark ? "dark" : "light";
};

const ThemeContext = createContext<ThemeContextType | undefined>(undefined);

export const ThemeProvider = ({ children }: { children: ReactNode }) => {
  const [theme, setTheme] = useState<Theme>(getInitialTheme);

  useEffect(() => {
    applyDomTheme(theme);
    if (isBrowser) {
      localStorage.setItem(STORAGE_KEY, theme);
    }
  }, [theme]);

  const toggleTheme = useCallback(() => {
    setTheme((prev) => (prev === "light" ? "dark" : "light"));
  }, []);

  const palettes = useMemo(
    () => ({
      light: {
        bg: "bg-gray-50",
        bgSecondary: "bg-white",
        bgTertiary: "bg-gray-100",
        text: "text-gray-900",
        textSecondary: "text-gray-700",
        textMuted: "text-gray-600",
        border: "border-gray-200",
        borderMuted: "border-gray-300",
        accent: "text-blue-600"
      },
      dark: {
        bg: "bg-gray-900",
        bgSecondary: "bg-gray-800",
        bgTertiary: "bg-gray-700",
        text: "text-white",
        textSecondary: "text-gray-300",
        textMuted: "text-gray-400",
        border: "border-gray-700",
        borderMuted: "border-gray-600",
        accent: "text-blue-400"
      }
    }),
    []
  );

  const getThemeClasses = useCallback(() => palettes[theme], [palettes, theme]);

  const value = useMemo(() => ({ theme, toggleTheme, getThemeClasses }), [theme, toggleTheme, getThemeClasses]);

  return <ThemeContext.Provider value={value}>{children}</ThemeContext.Provider>;
};

export const useTheme = () => {
  const context = useContext(ThemeContext);

  if (!context) {
    throw new Error("useTheme must be used within a ThemeProvider");
  }

  return context;
};
