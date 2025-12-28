type MimeType = "image/png" | "image/jpeg" | "image/bmp" | "application/octet-stream";

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

export const getMimeTypeFromFilename = (filename: string): MimeType => {
  const normalized = filename.toLowerCase();
  if (normalized.endsWith(".png")) return "image/png";
  if (normalized.endsWith(".jpg") || normalized.endsWith(".jpeg")) return "image/jpeg";
  if (normalized.endsWith(".bmp")) return "image/bmp";
  return "application/octet-stream";
};

export const basename = (path: string | null | undefined): string => {
  if (!path) return "";
  return path.split(/[/\\]/).pop() ?? "";
};
