/**
 * @file format.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief String formatting helpers.
 * @description Provides utilities for capitalization, status text formatting, and algorithm name normalization.
 * @version 1.0
 * @date 2026-03-16
 */

/** @brief Capitalizes first character of a string. */
export const capitalizeFirst = (str: string): string => {
  if (!str) return str;
  return str.charAt(0).toUpperCase() + str.slice(1);
};

/** @brief Converts underscored status identifiers to human-readable text. */
export const formatStatus = (status: string): string =>
  status
    .split("_")
    .map((word) => capitalizeFirst(word))
    .join(" ");

/** @brief Normalizes algorithm identifiers by replacing separators with spaces. */
export const formatAlgorithmName = (algorithm: string): string => algorithm.replace(/[_-]/g, " ");
