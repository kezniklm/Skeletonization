import { and, asc, desc, eq } from "drizzle-orm";

import { db } from "@/database";
import { runImage, type runImageStatusEnum } from "@/database/schema/run-image";
import { type InsertRunImage, type UpdateRunImage } from "@/database/zod/run-image";

export const getRunImageById = async (runImageId: string) => {
  const [result] = await db.select().from(runImage).where(eq(runImage.id, runImageId));

  return result ?? null;
};

export const getRunImagesByRunId = async (runId: string) =>
  db.select().from(runImage).where(eq(runImage.runId, runId)).orderBy(asc(runImage.ordinal));

export const getRunImagesByImageId = async (imageId: string) =>
  db.select().from(runImage).where(eq(runImage.imageId, imageId)).orderBy(desc(runImage.createdAt));

export const getRunImagesByStatus = async (status: (typeof runImageStatusEnum)[number]) =>
  db.select().from(runImage).where(eq(runImage.status, status)).orderBy(desc(runImage.createdAt));

export const getRunImageByRunAndImage = async (runId: string, imageId: string) => {
  const [result] = await db
    .select()
    .from(runImage)
    .where(and(eq(runImage.runId, runId), eq(runImage.imageId, imageId)));

  return result ?? null;
};

export const createRunImage = async (runImageData: InsertRunImage) => {
  const [newRunImage] = await db.insert(runImage).values(runImageData).returning();

  return newRunImage;
};

export const createRunImagesBulk = async (runImagesData: InsertRunImage[]) => {
  if (runImagesData.length === 0) return [];
  const newRunImages = await db.insert(runImage).values(runImagesData).returning();
  return newRunImages;
};

export const updateRunImage = async (runImageId: string, runImageData: Partial<UpdateRunImage>) => {
  const [updated] = await db.update(runImage).set(runImageData).where(eq(runImage.id, runImageId)).returning();

  return updated;
};

export const deleteRunImage = async (runImageId: string) => {
  await db.delete(runImage).where(eq(runImage.id, runImageId));
};

export const updateRunImageStatus = async (runImageId: string, status: (typeof runImageStatusEnum)[number]) => {
  const updates: Partial<UpdateRunImage> = { status };

  if (status === "completed" || status === "failed") {
    updates.processedAt = new Date();
  }

  return updateRunImage(runImageId, updates);
};

export const incrementRunImageAttempt = async (runImageId: string) => {
  const current = await getRunImageById(runImageId);
  if (!current) return null;

  return updateRunImage(runImageId, { attempt: (current.attempt ?? 0) + 1 });
};
