import type { SelectImage } from "@/database/zod/image";

import { type SizeFilter } from "./types";

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

export const formatFileSize = (bytes: number): string => {
  if (bytes < 1024) return `${bytes} B`;
  if (bytes < 1024 * 1024) return `${(bytes / 1024).toFixed(1)} KB`;
  return `${(bytes / (1024 * 1024)).toFixed(1)} MB`;
};

export const formatDate = (date: Date | string | null | undefined): string => {
  if (!date) return "N/A";
  return new Date(date).toLocaleDateString("en-US", {
    month: "short",
    day: "numeric",
    year: "numeric"
  });
};

export const getStatusLabel = (status: SelectImage["status"]): string =>
  status.charAt(0).toUpperCase() + status.slice(1);

export const getStatusBadgeClass = (status: SelectImage["status"]): string => {
  switch (status) {
    case "validated":
      return "border-green-200 bg-green-50 text-green-700 dark:border-green-800 dark:bg-green-950 dark:text-green-400";
    case "archived":
      return "border-gray-200 bg-gray-50 text-gray-700 dark:border-gray-800 dark:bg-gray-950 dark:text-gray-400";
    case "derived":
      return "border-purple-200 bg-purple-50 text-purple-700 dark:border-purple-800 dark:bg-purple-950 dark:text-purple-400";
    default:
      return "border-blue-200 bg-blue-50 text-blue-700 dark:border-blue-800 dark:bg-blue-950 dark:text-blue-400";
  }
};
