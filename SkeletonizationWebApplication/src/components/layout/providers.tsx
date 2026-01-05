"use client";

import { type PropsWithChildren } from "react";
import { Toaster } from "sonner";
import { QueryClient, QueryClientProvider } from "@tanstack/react-query";

import { ThemeProvider } from "@/contexts/theme-context";
import { CompactModeProvider } from "@/contexts/compact-mode-context";
import { RunNotificationsProvider } from "@/contexts/run-notifications-context";
import { TimezoneProvider } from "@/contexts/timezone-context";

import { AnimatedBackgroundProvider } from "./animated-background";

const queryClient = new QueryClient();

type ProvidersProps = PropsWithChildren<{
  initialTheme?: "light" | "dark" | "system";
  compactMode?: boolean;
  isLoggedIn?: boolean;
  timezone?: string;
}>;

export const Providers = ({
  children,
  initialTheme = "system",
  compactMode = false,
  isLoggedIn = false,
  timezone
}: ProvidersProps) => (
  <QueryClientProvider client={queryClient}>
    <ThemeProvider initialTheme={initialTheme}>
      <CompactModeProvider initialCompactMode={compactMode}>
        <TimezoneProvider initialTimezone={timezone}>
          <AnimatedBackgroundProvider>
            <RunNotificationsProvider enabled={isLoggedIn}>{children}</RunNotificationsProvider>
            <Toaster richColors />
          </AnimatedBackgroundProvider>
        </TimezoneProvider>
      </CompactModeProvider>
    </ThemeProvider>
  </QueryClientProvider>
);
