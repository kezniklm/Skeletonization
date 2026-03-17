/**
 * @file utils.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Provides helper formatting utilities for image gallery UI.
 * @description Includes size labeling, date formatting, status text conversion, and status badge style mapping.
 * @version 1.0
 * @date 2026-03-16
 */

import { type SizeFilter } from "@/components/filters";
import type { SelectImage } from "@/database/zod/image";
import { formatDateInTimezone } from "@/lib/date-time";

/**
 * @brief Converts size filter key into readable megapixel label.
 * @param size Size filter key.
 * @returns Human-readable size range label.
 */
export const getSizeLabel = (size: SizeFilter): string => {
  switch (size) {
    case "small":
      return "< 1MP";
    case "medium":
      return "1-4MP";
    case "large":
      return "4-10MP";
    case "xlarge":
      return "> 10MP";
    default:
      return "";
  }
};

/**
 * @brief Formats byte count into compact readable units.
 * @param bytes Raw byte count.
 * @returns Formatted file size string.
 */
export const formatFileSize = (bytes: number): string => {
  if (bytes < 1024) return `${bytes} B`;
  if (bytes < 1024 * 1024) return `${(bytes / 1024).toFixed(1)} KB`;
  return `${(bytes / (1024 * 1024)).toFixed(1)} MB`;
};

/**
 * @brief Formats image date with optional timezone.
 * @param date Input date value.
 * @param timezone Optional timezone identifier.
 * @returns Localized date string.
 */
export const formatDate = (date: Date | string | null | undefined, timezone?: string | null): string =>
  formatDateInTimezone(
    date,
    timezone,
    {
      month: "short",
      day: "numeric",
      year: "numeric"
    },
    "en-US"
  );

/**
 * @brief Converts image status to capitalized label.
 * @param status Image status value.
 * @returns Human-readable status label.
 */
export const getStatusLabel = (status: SelectImage["status"]): string =>
  status.charAt(0).toUpperCase() + status.slice(1);

/**
 * @brief Returns badge class set for image status.
 * @param status Image status value.
 * @returns Tailwind class string for status badge.
 */
export const getStatusBadgeClass = (status: SelectImage["status"]): string => {
  switch (status) {
    case "skeletonized":
      return "border-green-200 bg-green-50 text-green-700 dark:border-green-800 dark:bg-green-950 dark:text-green-400";
    case "archived":
      return "border-gray-200 bg-gray-50 text-gray-700 dark:border-gray-800 dark:bg-gray-950 dark:text-gray-400";
    case "derived":
      return "border-purple-200 bg-purple-50 text-purple-700 dark:border-purple-800 dark:bg-purple-950 dark:text-purple-400";
    default:
      return "border-blue-200 bg-blue-50 text-blue-700 dark:border-blue-800 dark:bg-blue-950 dark:text-blue-400";
  }
};
