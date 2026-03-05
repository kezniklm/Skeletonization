"use client";

import { type PropsWithChildren } from "react";
import { Toaster } from "sonner";
import { QueryClient, QueryClientProvider } from "@tanstack/react-query";

import { ThemeProvider } from "@/contexts/theme-context";
import { AnimatedBackgroundPreferenceProvider } from "@/contexts/animated-background-preference-context";
import { RunNotificationsProvider } from "@/contexts/run-notifications-context";
import { TimezoneProvider } from "@/contexts/timezone-context";

import { AnimatedBackgroundProvider } from "./animated-background";

const queryClient = new QueryClient();

type ProvidersProps = PropsWithChildren<{
  initialTheme?: "light" | "dark" | "system";
  animatedBackgroundDisabled?: boolean;
  isLoggedIn?: boolean;
  timezone?: string;
}>;

export const Providers = ({
  children,
  initialTheme = "system",
  animatedBackgroundDisabled = false,
  isLoggedIn = false,
  timezone
}: ProvidersProps) => (
  <QueryClientProvider client={queryClient}>
    <ThemeProvider initialTheme={initialTheme}>
      <AnimatedBackgroundPreferenceProvider initialAnimatedBackgroundDisabled={animatedBackgroundDisabled}>
        <TimezoneProvider initialTimezone={timezone}>
          <AnimatedBackgroundProvider>
            <RunNotificationsProvider enabled={isLoggedIn}>{children}</RunNotificationsProvider>
            <Toaster richColors />
          </AnimatedBackgroundProvider>
        </TimezoneProvider>
      </AnimatedBackgroundPreferenceProvider>
    </ThemeProvider>
  </QueryClientProvider>
);
