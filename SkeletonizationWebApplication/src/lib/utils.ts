/**
 * @file utils.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Shared UI utility helpers.
 * @description Provides className merging utility for Tailwind and conditional class composition.
 * @version 1.0
 * @date 2026-03-16
 */

import { clsx, type ClassValue } from "clsx";
import { twMerge } from "tailwind-merge";

/** @brief Merges and deduplicates conditional className values. */
export const cn = (...inputs: ClassValue[]) => twMerge(clsx(inputs));
