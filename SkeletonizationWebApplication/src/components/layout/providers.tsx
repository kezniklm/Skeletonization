/**
 * @file providers.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Composes global client-side providers.
 * @description Wraps application content with theme, query client, background preferences, notifications, and timezone providers.
 * @version 1.0
 * @date 2026-03-16
 */

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

/**
 * @brief Renders provider hierarchy for client app features.
 * @description Centralizes context providers and toaster setup for shared cross-application state and behavior.
 * @param children App content to wrap.
 * @param initialTheme Initial theme mode preference.
 * @param animatedBackgroundDisabled Initial animated background toggle state.
 * @param isLoggedIn Whether run notifications should be enabled.
 * @param timezone Initial timezone value.
 * @returns Wrapped children with provider stack.
 */
/** @brief Composes global client-side providers around app content. */
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
