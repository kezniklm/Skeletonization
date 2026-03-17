/**
 * @file types.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Defines filter and sort type aliases.
 * @description Provides shared literal-union models for filtering and sorting image collections.
 * @version 1.0
 * @date 2026-03-16
 */

import { type FileOutputFormat } from "@/database/zod";

/**
 * @brief Supported sort order options.
 */
export type SortOption = "date-desc" | "date-asc" | "name-asc" | "name-desc";
/**
 * @brief Supported high-level image status filters.
 */
export type FilterType = "all" | "uploaded" | "skeletonized" | "archived" | "derived";
/**
 * @brief Supported image size category filters.
 */
export type SizeFilter = "small" | "medium" | "large" | "xlarge";
/**
 * @brief Lowercase image format values derived from output format enum.
 */
export type ImageFormat = Lowercase<FileOutputFormat>;
