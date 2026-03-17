/**
 * @file timezone-utils.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Timezone detection, validation, and labeling helpers.
 * @description Provides runtime timezone resolution and option list generation for settings and formatting flows.
 * @version 1.0
 * @date 2026-03-16
 */

/**
 * @brief Select-option model for timezone picker.
 */
export type TimezoneOption = { value: string; label: string };

/** @brief Detects browser timezone with fallback to default value. */
export const detectTimezone = (defaultTimezone: string) => {
  try {
    const tz = Intl.DateTimeFormat().resolvedOptions().timeZone;
    return isValidTimezone(tz) ? tz : defaultTimezone;
  } catch {
    return defaultTimezone;
  }
};

/** @brief Validates whether a timezone identifier is supported. */
export const isValidTimezone = (timezone: string): boolean => {
  if (typeof timezone !== "string" || timezone.trim().length === 0) return false;

  try {
    new Intl.DateTimeFormat("en-US", { timeZone: timezone });
    return true;
  } catch {
    return false;
  }
};

const titleCaseWord = (word: string) => {
  if (word.length === 0) return word;
  return word[0].toUpperCase() + word.slice(1).toLowerCase();
};

/** @brief Converts timezone id into user-facing label text. */
export const formatTimezoneLabel = (timezone: string): string => {
  if (timezone === "UTC") return "UTC";

  const [regionRaw, ...rest] = timezone.split("/");
  const region = regionRaw ? titleCaseWord(regionRaw.replaceAll("_", " ")) : "";
  const city = rest.join("/").replaceAll("_", " ");

  if (!region) return timezone;
  if (!city) return region;
  return `${city} (${region})`;
};

/** @brief Returns sorted timezone options for selectors. */
export const getTimezoneOptions = (): TimezoneOption[] => {
  try {
    const supportedValuesOf = (Intl as unknown as { supportedValuesOf?: (key: string) => string[] }).supportedValuesOf;
    const timezones = supportedValuesOf?.("timeZone") ?? ["UTC"];

    return [...new Set(timezones)]
      .filter(isValidTimezone)
      .sort()
      .map((tz) => ({ value: tz, label: formatTimezoneLabel(tz) }));
  } catch {
    return [{ value: "UTC", label: "UTC" }];
  }
};
