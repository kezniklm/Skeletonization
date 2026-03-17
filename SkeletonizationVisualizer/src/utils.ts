/**
 * @file utils.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Implements image download and archive export helpers.
 * @description Converts in-memory benchmark image payloads into downloadable PNG files and ZIP archives.
 * @version 1.0
 * @date 2026-03-16
 */

import JSZip from "jszip";

import type { BenchmarkData, ImageData } from "./types";

/**
 * @brief Triggers a browser download for the given resource URL.
 * @param href Downloadable resource URL.
 * @param filename Suggested output filename.
 * @returns No return value.
 */
const createDownloadLink = (href: string, filename: string): void => {
  const link = document.createElement("a");
  link.href = href;
  link.download = filename;
  document.body.appendChild(link);
  link.click();
  document.body.removeChild(link);
};

/**
 * @brief Converts base64-encoded image content into binary bytes.
 * @param base64Data Base64 image content without data URL prefix.
 * @returns Binary array suitable for ZIP file insertion.
 */
const base64ToUint8Array = (base64Data: string): Uint8Array => {
  const binaryData = atob(base64Data);
  const bytes = new Uint8Array(binaryData.length);
  for (let i = 0; i < binaryData.length; i++) {
    bytes[i] = binaryData.charCodeAt(i);
  }
  return bytes;
};

/**
 * @brief Builds a sanitized PNG filename from image metadata.
 * @param label Optional image label.
 * @param id Image identifier fallback.
 * @param index Optional index prefix for ordering.
 * @returns Sanitized filename ending with .png.
 */
const createSafeFilename = (label: string | undefined, id: string, index?: number): string => {
  let filename = label ?? `Image_${index ?? id}`;
  filename = filename.replace(/\s+/g, "_").replace(/[^a-zA-Z0-9_-]/g, "_");
  if (index !== undefined) {
    filename = `${String(index).padStart(2, "0")}_${filename}`;
  }
  return `${filename}.png`;
};

/**
 * @brief Downloads one benchmark image as a PNG file.
 * @param image Image payload to export.
 * @returns No return value.
 * @example
 * downloadImage(selectedImage);
 */
export const downloadImage = (image: ImageData): void => {
  const filename = createSafeFilename(image.label, image.id);
  const href = `data:image/png;base64,${image.data}`;
  createDownloadLink(href, filename);
};

/**
 * @brief Exports all benchmark images into a structured ZIP archive.
 * @param data Benchmark dataset containing containers and images.
 * @returns Promise resolved after archive generation and download trigger.
 * @example
 * await exportAllImages(data);
 */
export const exportAllImages = async (data: BenchmarkData): Promise<void> => {
  if (!data) {
    return;
  }

  try {
    const zip = new JSZip();

    // Organize images by container
    for (const container of data.containers) {
      const folderName = container.name.replace(/\s+/g, "_").replace(/[^a-zA-Z0-9_-]/g, "");
      const folder = zip.folder(folderName);

      if (!folder) {
        continue;
      }

      // Add each image to the container's folder
      for (let i = 0; i < container.images.length; i++) {
        const image = container.images[i];
        const filename = createSafeFilename(image.label, image.id, i);
        const imageBytes = base64ToUint8Array(image.data);
        folder.file(filename, imageBytes);
      }
    }

    // Generate and download ZIP file
    const content = await zip.generateAsync({ type: "blob" });
    const filename = `skeletonization_benchmark_${new Date().toISOString().split("T")[0]}.zip`;
    const href = URL.createObjectURL(content);

    createDownloadLink(href, filename);

    // Clean up the URL object
    URL.revokeObjectURL(href);

    console.log("✅ Export completed successfully!");
  } catch (error) {
    console.error("❌ Error exporting images:", error);
    alert("Error exporting images. Please try again.");
  }
};
