/**
 * @file files.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief File naming utility helpers.
 * @description Provides helper to generate collision-resistant randomized filenames.
 * @version 1.0
 * @date 2026-03-16
 */

/** @brief Generates randomized filename preserving original extension. */
export const generateRandomFilename = (originalName: string): string => {
  const nameParts = originalName.split(".");
  const ext = nameParts.pop();
  const baseName = nameParts.join(".");
  const randomString = Math.random().toString(36).substring(2, 10);
  const timestamp = Date.now();
  return `${baseName}_${timestamp}_${randomString}.${ext}`;
};
