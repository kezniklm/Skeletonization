"use client";

import { type PropsWithChildren } from "react";
import { Toaster } from "sonner";
import { QueryClient, QueryClientProvider } from "@tanstack/react-query";

import { ThemeProvider } from "@/contexts/theme-context";
import { CompactModeProvider } from "@/contexts/compact-mode-context";
import { RunNotificationsProvider } from "@/contexts/run-notifications-context";

import { AnimatedBackgroundProvider } from "./animated-background";

const queryClient = new QueryClient();

type ProvidersProps = PropsWithChildren<{
  initialTheme?: "light" | "dark" | "system";
  compactMode?: boolean;
  isLoggedIn?: boolean;
}>;

export const Providers = ({
  children,
  initialTheme = "system",
  compactMode = false,
  isLoggedIn = false
}: ProvidersProps) => (
  <QueryClientProvider client={queryClient}>
    <ThemeProvider initialTheme={initialTheme}>
      <CompactModeProvider initialCompactMode={compactMode}>
        <AnimatedBackgroundProvider>
          <RunNotificationsProvider enabled={isLoggedIn}>{children}</RunNotificationsProvider>
          <Toaster richColors />
        </AnimatedBackgroundProvider>
      </CompactModeProvider>
    </ThemeProvider>
  </QueryClientProvider>
);
