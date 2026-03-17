/**
 * @file date-time.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Timezone-aware date and datetime format helpers.
 * @description Normalizes dates and timezones and exposes formatting utilities for UI display.
 * @version 1.0
 * @date 2026-03-16
 */

import { isValidTimezone } from "@/lib/timezone-utils";

const normalizeTimezone = (timezone?: string | null): string => {
  if (timezone && isValidTimezone(timezone)) return timezone;
  return "UTC";
};

const toDate = (value: Date | string | null | undefined): Date | null => {
  if (!value) return null;
  const date = value instanceof Date ? value : new Date(value);
  if (Number.isNaN(date.getTime())) return null;
  return date;
};

/** @brief Formats a date value using specified or normalized timezone. */
export const formatDateInTimezone = (
  value: Date | string | null | undefined,
  timezone?: string | null,
  options: Intl.DateTimeFormatOptions = { year: "numeric", month: "long", day: "numeric" },
  locale?: string
): string => {
  const date = toDate(value);
  if (!date) return "N/A";

  const timeZone = normalizeTimezone(timezone);

  return new Intl.DateTimeFormat(locale, { ...options, timeZone }).format(date);
};

/** @brief Formats a date-time value using specified or normalized timezone. */
export const formatDateTimeInTimezone = (
  value: Date | string | null | undefined,
  timezone?: string | null,
  options: Intl.DateTimeFormatOptions = {
    year: "numeric",
    month: "short",
    day: "2-digit",
    hour: "2-digit",
    minute: "2-digit"
  },
  locale?: string
): string | null => {
  const date = toDate(value);
  if (!date) return null;

  const timeZone = normalizeTimezone(timezone);

  return new Intl.DateTimeFormat(locale, { ...options, timeZone }).format(date);
};
