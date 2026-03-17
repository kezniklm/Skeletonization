/**
 * @file images.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Server actions for image upload and lifecycle operations.
 * @description Implements authenticated image upload, metadata updates, archive operations, deletion, and preprocessing output persistence.
 * @version 1.0
 * @date 2026-03-16
 */

"use server";

import { createHash } from "crypto";

import { getExtension, getMimeType } from "@/app/preprocessing/utils/format-mapper";
import { type FileOutputFormat } from "@/database/zod";
import { updateImageSchema } from "@/database/zod/image";
import { getStorage } from "@/lib/storage";
import { createImage, deleteImage, getImageByChecksum, getImageById, updateImage } from "@/repositories/image";

import { requireUser } from "./common";

const ALLOWED_IMAGE_TYPES = new Set(["image/png", "image/jpeg", "image/jpg", "image/bmp", "image/tiff"]);

const MAX_IMAGE_SIZE_BYTES = 10 * 1024 * 1024; // 10MB

const validateImageFile = (file: File) => {
  if (!file) {
    throw new Error("No file provided");
  }

  if (!ALLOWED_IMAGE_TYPES.has(file.type)) {
    throw new Error("Invalid file type. Only PNG, JPEG, BMP, and TIFF are allowed");
  }

  if (file.size > MAX_IMAGE_SIZE_BYTES) {
    throw new Error("File too large. Maximum size is 10MB");
  }
};

const getImageDimensions = async (buffer: Buffer, mimeType: string): Promise<{ width: number; height: number }> => {
  try {
    if (mimeType === "image/png") {
      const width = buffer.readUInt32BE(16);
      const height = buffer.readUInt32BE(20);
      return { width, height };
    }

    if (mimeType === "image/jpeg" || mimeType === "image/jpg") {
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
    }

    if (mimeType === "image/bmp") {
      const width = buffer.readInt32LE(18);
      const height = Math.abs(buffer.readInt32LE(22));
      return { width, height };
    }
  } catch (error) {
    console.error("Error reading image dimensions:", error);
  }

  return { width: 0, height: 0 };
};

const buildStorageKey = (userId: string, originalName: string) => {
  const timestamp = Date.now();
  const sanitizedFilename = originalName.replace(/[^a-zA-Z0-9.-]/g, "_");
  const filename = `${timestamp}_${sanitizedFilename}`;
  return `uploads/${userId}/${filename}`;
};

/** @brief Uploads a validated image file and creates image metadata record. */
export const uploadImageAction = async (file: File) => {
  const user = await requireUser("upload images");

  validateImageFile(file);

  const bytes = await file.arrayBuffer();
  const buffer = Buffer.from(bytes);

  const checksum = createHash("sha256").update(buffer).digest("hex");

  const existingImage = await getImageByChecksum(checksum, user.id);

  if (existingImage) {
    throw new Error("This image has already been uploaded");
  }

  const dimensions = await getImageDimensions(buffer, file.type);

  const storage = getStorage();
  const storageKey = buildStorageKey(user.id, file.name);
  const { url } = await storage.putObject(storageKey, buffer, { contentType: file.type });

  const imageRecord = await createImage({
    userId: user.id,
    originalFilename: file.name,
    storagePath: storageKey,
    url,
    mime: file.type,
    width: dimensions.width,
    height: dimensions.height,
    sizeBytes: file.size,
    checksum,
    status: "uploaded"
  });

  return imageRecord;
};

/** @brief Updates mutable image metadata fields for an image id. */
export const updateImageAction = async (imageId: string, imageData: { originalFilename?: string; status?: string }) => {
  await requireUser("update images");

  const parsedData = updateImageSchema.partial().parse({ ...imageData, id: imageId });

  return updateImage(imageId, parsedData);
};

/** @brief Archives a user-owned image. */
export const archiveImageAction = async (imageId: string) => {
  const user = await requireUser("archive images");

  const image = await getImageById(imageId);

  if (!image) {
    throw new Error("Image not found");
  }

  if (image.userId !== user.id) {
    throw new Error("You can only archive your own images");
  }

  return updateImage(imageId, { status: "archived" });
};

/** @brief Unarchives a user-owned image back to uploaded status. */
export const unarchiveImageAction = async (imageId: string) => {
  const user = await requireUser("unarchive images");

  const image = await getImageById(imageId);

  if (!image) {
    throw new Error("Image not found");
  }

  if (image.userId !== user.id) {
    throw new Error("You can only unarchive your own images");
  }

  return updateImage(imageId, { status: "uploaded" });
};

/** @brief Deletes a user-owned image from storage and database. */
export const deleteImageAction = async (imageId: string) => {
  const user = await requireUser("delete images");

  const image = await getImageById(imageId);

  if (!image) {
    throw new Error("Image not found");
  }

  if (image.userId !== user.id) {
    throw new Error("You can only delete your own images");
  }

  try {
    const storage = getStorage();
    await storage.deleteObject(image.storagePath);
  } catch (error) {
    console.error("Error deleting file:", error);
  }

  await deleteImage(imageId);
};

/** @brief Saves preprocessing output as derived image for a source image. */
export const savePreprocessedImageAction = async (
  imageId: string,
  imageDataUrl: string,
  outputFormat: FileOutputFormat = "PNG"
) => {
  const user = await requireUser("save preprocessed images");

  const originalImage = await getImageById(imageId);

  if (!originalImage) {
    throw new Error("Original image not found");
  }

  if (originalImage.userId !== user.id) {
    throw new Error("You can only preprocess your own images");
  }

  const base64Data = imageDataUrl.replace(/^data:image\/\w+;base64,/, "");
  const buffer = Buffer.from(base64Data, "base64");

  const checksum = createHash("sha256").update(buffer).digest("hex");

  const existingImage = await getImageByChecksum(checksum, user.id);

  if (existingImage) {
    return existingImage;
  }

  const timestamp = Date.now();
  const sanitizedFilename = originalImage.originalFilename.replace(/\.(png|jpg|jpeg|bmp|tiff)$/i, "");
  const fileExtension = getExtension(outputFormat);
  const mimeType = getMimeType(outputFormat);
  const filename = `${timestamp}_${sanitizedFilename}_preprocessed.${fileExtension}`;

  const storage = getStorage();
  const storageKey = `uploads/${user.id}/${filename}`;
  const { url } = await storage.putObject(storageKey, buffer, { contentType: mimeType });

  const dimensions = await getImageDimensions(buffer, mimeType);

  const imageRecord = await createImage({
    userId: user.id,
    originalFilename: `${sanitizedFilename}_preprocessed.${fileExtension}`,
    storagePath: storageKey,
    url,
    mime: mimeType,
    width: dimensions.width,
    height: dimensions.height,
    sizeBytes: buffer.length,
    checksum,
    status: "derived",
    parentImageId: imageId
  });

  return imageRecord;
};
