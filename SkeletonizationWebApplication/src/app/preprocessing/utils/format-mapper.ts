/**
 * @file format-mapper.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Maps output formats to MIME types and file extensions.
 * @description Provides lookup tables and helper functions used by preprocessing save and download actions.
 */

import { type FileOutputFormat } from "@/database/zod";

/**
 * @brief Maps file output formats to browser MIME types.
 */
export const formatToMimeType: Record<FileOutputFormat, string> = {
  PNG: "image/png",
  JPEG: "image/jpeg",
  TIFF: "image/tiff",
  BMP: "image/bmp"
};

/**
 * @brief Maps file output formats to filename extensions.
 */
export const formatToExtension: Record<FileOutputFormat, string> = {
  PNG: "png",
  JPEG: "jpg",
  TIFF: "tiff",
  BMP: "bmp"
};

/**
 * @brief Resolves MIME type for an output format.
 * @description Normalizes format keys and falls back to PNG when unknown.
 * @param format Format string to resolve.
 * @returns MIME type string for the format.
 */
export const getMimeType = (format: string): string => {
  const normalized = format.toUpperCase();
  return formatToMimeType[normalized as FileOutputFormat] ?? "image/png";
};

/**
 * @brief Resolves file extension for an output format.
 * @description Normalizes format keys and falls back to PNG extension when unknown.
 * @param format Format string to resolve.
 * @returns File extension string for the format.
 */
export const getExtension = (format: string): string => {
  const normalized = format.toUpperCase();
  return formatToExtension[normalized as FileOutputFormat] ?? "png";
};
