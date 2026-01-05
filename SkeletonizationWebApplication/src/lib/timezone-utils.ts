export type TimezoneOption = { value: string; label: string };

export const detectTimezone = (defaultTimezone: string) => {
  try {
    const tz = Intl.DateTimeFormat().resolvedOptions().timeZone;
    return isValidTimezone(tz) ? tz : defaultTimezone;
  } catch {
    return defaultTimezone;
  }
};

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

export const formatTimezoneLabel = (timezone: string): string => {
  if (timezone === "UTC") return "UTC";

  const [regionRaw, ...rest] = timezone.split("/");
  const region = regionRaw ? titleCaseWord(regionRaw.replaceAll("_", " ")) : "";
  const city = rest.join("/").replaceAll("_", " ");

  if (!region) return timezone;
  if (!city) return region;
  return `${city} (${region})`;
};

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
