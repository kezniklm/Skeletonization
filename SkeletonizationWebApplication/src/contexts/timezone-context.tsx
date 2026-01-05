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

export const useTimezone = () => {
  const context = useContext(TimezoneContext);

  if (!context) {
    throw new Error("useTimezone must be used within a TimezoneProvider");
  }

  return context;
};
