import { desc, eq } from "drizzle-orm";

import { db } from "@/database";
import { algorithm } from "@/database/schema/algorithm";
import { type InsertAlgorithm, type UpdateAlgorithm } from "@/database/zod/algorithm";

export const getAlgorithmById = async (algorithmId: string) => {
  const [result] = await db.select().from(algorithm).where(eq(algorithm.id, algorithmId));

  return result ?? null;
};

export const getAllAlgorithms = async () => db.select().from(algorithm).orderBy(desc(algorithm.createdAt));

export const getAlgorithmByName = async (name: string) => {
  const [result] = await db.select().from(algorithm).where(eq(algorithm.name, name));

  return result ?? null;
};

export const createAlgorithm = async (algorithmData: InsertAlgorithm) => {
  const [newAlgorithm] = await db.insert(algorithm).values(algorithmData).returning();

  return newAlgorithm;
};

export const updateAlgorithm = async (algorithmId: string, algorithmData: Partial<UpdateAlgorithm>) => {
  const [updated] = await db.update(algorithm).set(algorithmData).where(eq(algorithm.id, algorithmId)).returning();

  return updated;
};

export const deleteAlgorithm = async (algorithmId: string) => {
  await db.delete(algorithm).where(eq(algorithm.id, algorithmId));
};
