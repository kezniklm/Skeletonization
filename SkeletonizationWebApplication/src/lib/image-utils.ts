/**
 * @file image-utils.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Image metadata utility helpers.
 * @description Provides MIME detection, dimension extraction, and basename helpers for output processing.
 * @version 1.0
 * @date 2026-03-16
 */

type MimeType = "image/png" | "image/jpeg" | "image/bmp" | "image/tiff" | "application/octet-stream";

/**
 * @brief Represents image width and height values.
 */
export type ImageDimensions = {
  width: number;
  height: number;
};

const readPngDimensions = (buffer: Buffer): ImageDimensions => {
  const width = buffer.readUInt32BE(16);
  const height = buffer.readUInt32BE(20);
  return { width, height };
};

const readJpegDimensions = (buffer: Buffer): ImageDimensions => {
  let offset = 2;
  while (offset < buffer.length) {
    if (buffer[offset] !== 0xff) break;
    const marker = buffer[offset + 1];
    const size = buffer.readUInt16BE(offset + 2);

    if (marker === 0xc0 || marker === 0xc2) {
      const height = buffer.readUInt16BE(offset + 5);
      const width = buffer.readUInt16BE(offset + 7);
      return { width, height };
    }

    offset += 2 + size;
  }
  return { width: 0, height: 0 };
};

const readBmpDimensions = (buffer: Buffer): ImageDimensions => {
  const width = buffer.readInt32LE(18);
  const height = Math.abs(buffer.readInt32LE(22));
  return { width, height };
};

/** @brief Extracts image dimensions from raw buffer based on MIME type. */
export const getImageDimensions = (buffer: Buffer, mimeType: string): ImageDimensions => {
  try {
    switch (mimeType) {
      case "image/png":
        return readPngDimensions(buffer);
      case "image/jpeg":
      case "image/jpg":
        return readJpegDimensions(buffer);
      case "image/bmp":
        return readBmpDimensions(buffer);
      default:
        return { width: 0, height: 0 };
    }
  } catch (error) {
    console.error("Error reading image dimensions:", error);
    return { width: 0, height: 0 };
  }
};

/** @brief Infers MIME type from a filename extension. */
export const getMimeTypeFromFilename = (filename: string): MimeType => {
  const normalized = filename.toLowerCase();
  if (normalized.endsWith(".png")) return "image/png";
  if (normalized.endsWith(".jpg") || normalized.endsWith(".jpeg")) return "image/jpeg";
  if (normalized.endsWith(".bmp")) return "image/bmp";
  if (normalized.endsWith(".tif") || normalized.endsWith(".tiff")) return "image/tiff";
  return "application/octet-stream";
};

/** @brief Returns last path segment from path-like string. */
export const basename = (path: string | null | undefined): string => {
  if (!path) return "";
  return path.split(/[/\\]/).pop() ?? "";
};
