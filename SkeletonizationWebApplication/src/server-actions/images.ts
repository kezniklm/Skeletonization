"use server";

import { createHash } from "crypto";
import { mkdir, unlink, writeFile } from "fs/promises";
import { join } from "path";

import { updateImageSchema } from "@/database/zod/image";
import { createImage, deleteImage, getImageByChecksum, getImageById, updateImage } from "@/repositories/image";

import { requireUser } from "./common";

const ALLOWED_IMAGE_TYPES = new Set(["image/png", "image/jpeg", "image/jpg", "image/bmp", "image/tiff"]);

const MAX_IMAGE_SIZE_BYTES = 10 * 1024 * 1024; // 10MB
const PUBLIC_UPLOADS_DIR = "uploads";

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

const buildStoragePaths = (userId: string, originalName: string) => {
  const timestamp = Date.now();
  const sanitizedFilename = originalName.replace(/[^a-zA-Z0-9.-]/g, "_");
  const filename = `${timestamp}_${sanitizedFilename}`;

  const storagePath = join(PUBLIC_UPLOADS_DIR, userId, filename);
  const fullPath = join(process.cwd(), "public", storagePath);
  const imageUrl = `/${storagePath.replace(/\\/g, "/")}`;

  return { filename, storagePath, fullPath, imageUrl };
};

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

  const { storagePath, fullPath, imageUrl } = buildStoragePaths(user.id, file.name);

  const userUploadDir = join(process.cwd(), "public", PUBLIC_UPLOADS_DIR, user.id);

  await mkdir(userUploadDir, { recursive: true });

  await writeFile(fullPath, buffer);

  const imageRecord = await createImage({
    userId: user.id,
    originalFilename: file.name,
    storagePath,
    url: imageUrl,
    mime: file.type,
    width: dimensions.width,
    height: dimensions.height,
    sizeBytes: file.size,
    checksum,
    status: "uploaded"
  });

  return imageRecord;
};

export const updateImageAction = async (imageId: string, imageData: { originalFilename?: string; status?: string }) => {
  await requireUser("update images");

  const validatedData = updateImageSchema.partial().parse({ ...imageData, id: imageId });

  return updateImage(imageId, validatedData);
};

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

export const deleteImageAction = async (imageId: string) => {
  const user = await requireUser("delete images");

  const image = await getImageById(imageId);

  if (!image) {
    throw new Error("Image not found");
  }

  if (image.userId !== user.id) {
    throw new Error("You can only delete your own images");
  }

  const fullPath = join(process.cwd(), "public", image.storagePath);

  try {
    await unlink(fullPath);
  } catch (error) {
    console.error("Error deleting file:", error);
  }

  await deleteImage(imageId);
};
