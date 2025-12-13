import { and, asc, desc, eq, inArray, lt, or } from "drizzle-orm";

import { db } from "@/database";
import { image, type imageStatusEnum } from "@/database/schema/image";
import { type InsertImage, type UpdateImage } from "@/database/zod/image";

export const getImageById = async (imageId: string) => {
  const [result] = await db.select().from(image).where(eq(image.id, imageId));

  return result ?? null;
};

export const getImageByIdAndUserId = async (imageId: string | undefined, userId: string) => {
  if (!imageId) {
    return null;
  }

  const [result] = await db
    .select()
    .from(image)
    .where(and(eq(image.id, imageId), eq(image.userId, userId)));

  return result ?? null;
};

export const getImagesByUserId = async (userId: string) =>
  db.select().from(image).where(eq(image.userId, userId)).orderBy(desc(image.createdAt));

export const getImagesByIds = async (ids: string[]) => db.select().from(image).where(inArray(image.id, ids));

export const getImagesCountByUserId = async (userId: string) => db.$count(image, eq(image.userId, userId));

export const getImagesByUserIdPaginated = async (
  userId: string,
  options: {
    limit?: number;
    cursor?: { createdAt: Date; id: string };
    sortBy?: "createdAt" | "originalFilename";
    sortOrder?: "asc" | "desc";
  } = {}
) => {
  const { limit = 20, cursor, sortBy = "createdAt", sortOrder = "desc" } = options;

  const conditions = [eq(image.userId, userId)];

  if (cursor) {
    if (sortBy === "createdAt") {
      conditions.push(
        sortOrder === "desc"
          ? or(
              lt(image.createdAt, cursor.createdAt),
              and(eq(image.createdAt, cursor.createdAt), lt(image.id, cursor.id))
            )!
          : or(
              lt(image.createdAt, cursor.createdAt),
              and(eq(image.createdAt, cursor.createdAt), lt(image.id, cursor.id))
            )!
      );
    } else {
      conditions.push(
        sortOrder === "desc"
          ? or(
              lt(image.originalFilename, cursor.createdAt.toISOString()),
              and(eq(image.originalFilename, cursor.createdAt.toISOString()), lt(image.id, cursor.id))
            )!
          : or(
              lt(image.originalFilename, cursor.createdAt.toISOString()),
              and(eq(image.originalFilename, cursor.createdAt.toISOString()), lt(image.id, cursor.id))
            )!
      );
    }
  }

  const orderFn = sortOrder === "desc" ? desc : asc;
  const orderCol = sortBy === "createdAt" ? image.createdAt : image.originalFilename;

  return db
    .select()
    .from(image)
    .where(and(...conditions))
    .orderBy(orderFn(orderCol), sortOrder === "desc" ? desc(image.id) : asc(image.id))
    .limit(limit + 1);
};

export const getImagesByRunId = async (runId: string) =>
  db.select().from(image).where(eq(image.processingRunId, runId)).orderBy(desc(image.createdAt));

export const getImagesByParentId = async (parentImageId: string) =>
  db.select().from(image).where(eq(image.parentImageId, parentImageId)).orderBy(desc(image.createdAt));

export const getUserImagesByStatus = async (userId: string, status: (typeof imageStatusEnum)[number]) =>
  db
    .select()
    .from(image)
    .where(and(eq(image.userId, userId), eq(image.status, status)))
    .orderBy(desc(image.createdAt));

export const createImage = async (imageData: InsertImage) => {
  const [newImage] = await db.insert(image).values(imageData).returning();

  return newImage;
};

export const updateImage = async (imageId: string, imageData: Partial<UpdateImage>) => {
  const [updated] = await db.update(image).set(imageData).where(eq(image.id, imageId)).returning();

  return updated;
};

export const deleteImage = async (imageId: string) => {
  await db.delete(image).where(eq(image.id, imageId));
};

export const getImageByChecksum = async (checksum: string, userId: string) => {
  const [result] = await db
    .select()
    .from(image)
    .where(and(eq(image.checksum, checksum), eq(image.userId, userId)));

  return result ?? null;
};
