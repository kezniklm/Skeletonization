import { and, desc, eq } from "drizzle-orm";

import { db } from "@/database";
import { output, type outputTypeEnum } from "@/database/schema/output";
import { type InsertOutput, type UpdateOutput } from "@/database/zod/output";

export const getOutputById = async (outputId: string) => {
  const [result] = await db.select().from(output).where(eq(output.id, outputId));

  return result ?? null;
};

export const getOutputsByRunImageId = async (runImageId: string) =>
  db.select().from(output).where(eq(output.runImageId, runImageId)).orderBy(desc(output.createdAt));

export const getOutputsByType = async (runImageId: string, type: (typeof outputTypeEnum)[number]) =>
  db
    .select()
    .from(output)
    .where(and(eq(output.runImageId, runImageId), eq(output.type, type)))
    .orderBy(desc(output.createdAt));

export const getOutputByTypeAndRunImage = async (runImageId: string, type: (typeof outputTypeEnum)[number]) => {
  const [result] = await db
    .select()
    .from(output)
    .where(and(eq(output.runImageId, runImageId), eq(output.type, type)))
    .orderBy(desc(output.createdAt));

  return result ?? null;
};

export const createOutput = async (outputData: InsertOutput) => {
  const [newOutput] = await db.insert(output).values(outputData).returning();

  return newOutput;
};

export const updateOutput = async (outputId: string, outputData: Partial<UpdateOutput>) => {
  const [updated] = await db.update(output).set(outputData).where(eq(output.id, outputId)).returning();

  return updated;
};

export const deleteOutput = async (outputId: string) => {
  await db.delete(output).where(eq(output.id, outputId));
};

export const deleteOutputsByRunImageId = async (runImageId: string) => {
  await db.delete(output).where(eq(output.runImageId, runImageId));
};
