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
