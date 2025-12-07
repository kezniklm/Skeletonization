import { type FileOutputFormat } from "@/database/zod";

export const formatToMimeType: Record<FileOutputFormat, string> = {
  PNG: "image/png",
  JPEG: "image/jpeg",
  TIFF: "image/tiff",
  BMP: "image/bmp"
};

export const formatToExtension: Record<FileOutputFormat, string> = {
  PNG: "png",
  JPEG: "jpg",
  TIFF: "tiff",
  BMP: "bmp"
};

export const getMimeType = (format: string): string => {
  const normalized = format.toUpperCase();
  return formatToMimeType[normalized as FileOutputFormat] ?? "image/png";
};

export const getExtension = (format: string): string => {
  const normalized = format.toUpperCase();
  return formatToExtension[normalized as FileOutputFormat] ?? "png";
};
