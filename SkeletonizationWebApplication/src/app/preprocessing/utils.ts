/**
 * @file utils.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Provides utility helpers for preprocessing interactions.
 * @description Contains browser utility functions shared by preprocessing components and hooks.
 */

/**
 * @brief Triggers download of a data URL as an image file.
 * @description Creates a temporary anchor element to initiate client-side download then removes it from the document.
 * @param dataUrl Encoded image data URL to download.
 * @param filename Target filename for the downloaded image.
 * @returns No return value.
 */
export const downloadImage = (dataUrl: string, filename: string) => {
  const link = document.createElement("a");
  link.href = dataUrl;
  link.download = filename;
  document.body.appendChild(link);
  link.click();
  document.body.removeChild(link);
};
