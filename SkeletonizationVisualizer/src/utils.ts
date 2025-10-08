import JSZip from "jszip";

import type { BenchmarkData, ImageData } from "./types";

/**
 * Utility functions for downloading and exporting images
 */

/**
 * Create a download link element and trigger download
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
 * Convert base64 image data to Uint8Array for ZIP processing
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
 * Create a safe filename from a label or ID
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
 * Download a single image
 */
export const downloadImage = (image: ImageData): void => {
  const filename = createSafeFilename(image.label, image.id);
  const href = `data:image/png;base64,${image.data}`;
  createDownloadLink(href, filename);
};

/**
 * Export all images from benchmark data as a ZIP file
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
