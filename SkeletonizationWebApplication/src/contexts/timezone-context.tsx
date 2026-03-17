/**
 * @file timezone-context.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Manages user timezone state.
 * @description Provides timezone context with validation and normalization helpers for UTC and detected local timezones.
 * @version 1.0
 * @date 2026-03-16
 */

"use client";

import { createContext, useContext, useState, type PropsWithChildren } from "react";

import { detectTimezone, isValidTimezone } from "@/lib/timezone-utils";
import { defaultPreferences } from "@/database/schema";

type TimezoneContextType = {
  timezone: string;
  resolvedTimezone: string;
  setTimezone: (timezone: string) => void;
};

const TimezoneContext = createContext<TimezoneContextType | undefined>(undefined);

type TimezoneProviderProps = PropsWithChildren<{
  initialTimezone?: string;
}>;

/**
 * @brief Provides timezone context for date rendering.
 * @description Normalizes initial timezone and exposes validated timezone setter to descendants.
 * @param children Descendant nodes requiring timezone context.
 * @param initialTimezone Initial timezone preference.
 * @returns Timezone context provider wrapper.
 */
export const TimezoneProvider = ({
  children,
  initialTimezone = defaultPreferences.timezone
}: TimezoneProviderProps) => {
  const [timezone, setTimezoneState] = useState<string>(() => {
    const initialResolved =
      initialTimezone === "UTC"
        ? detectTimezone(defaultPreferences.timezone)
        : (initialTimezone ?? defaultPreferences.timezone);
    return isValidTimezone(initialResolved) ? initialResolved : defaultPreferences.timezone;
  });

  const resolvedTimezone = isValidTimezone(timezone) ? timezone : defaultPreferences.timezone;

  const setTimezone = (tz: string) => {
    const nextResolved = tz === "UTC" ? detectTimezone(defaultPreferences.timezone) : tz;
    setTimezoneState(isValidTimezone(nextResolved) ? nextResolved : defaultPreferences.timezone);
  };

  return (
    <TimezoneContext.Provider value={{ timezone, resolvedTimezone, setTimezone }}>{children}</TimezoneContext.Provider>
  );
};

/**
 * @brief Returns current timezone context.
 * @description Throws when called outside `TimezoneProvider`.
 * @returns Timezone context values and setter.
 */
export const useTimezone = () => {
  const context = useContext(TimezoneContext);

  if (!context) {
    throw new Error("useTimezone must be used within a TimezoneProvider");
  }

  return context;
};
