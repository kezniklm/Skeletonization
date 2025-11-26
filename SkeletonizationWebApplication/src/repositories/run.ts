import { and, desc, eq } from "drizzle-orm";

import { db } from "@/database";
import { run, type runStatusEnum } from "@/database/schema/run";
import { type InsertRun, type UpdateRun } from "@/database/zod/run";

export const getRunById = async (runId: string) => {
  const [result] = await db.select().from(run).where(eq(run.id, runId));

  return result ?? null;
};

export const getRunsByUserId = async (userId: string) =>
  db.select().from(run).where(eq(run.userId, userId)).orderBy(desc(run.createdAt));

export const getRunsByAlgorithmId = async (algorithmId: string) =>
  db.select().from(run).where(eq(run.algorithmId, algorithmId)).orderBy(desc(run.createdAt));

export const getRunsByStatus = async (status: (typeof runStatusEnum)[number]) =>
  db.select().from(run).where(eq(run.status, status)).orderBy(desc(run.createdAt));

export const getUserRunsByStatus = async (userId: string, status: (typeof runStatusEnum)[number]) =>
  db
    .select()
    .from(run)
    .where(and(eq(run.userId, userId), eq(run.status, status)))
    .orderBy(desc(run.createdAt));

export const createRun = async (runData: InsertRun) => {
  const [newRun] = await db.insert(run).values(runData).returning();

  return newRun;
};

export const updateRun = async (runId: string, runData: Partial<UpdateRun>) => {
  const [updated] = await db.update(run).set(runData).where(eq(run.id, runId)).returning();

  return updated;
};

export const deleteRun = async (runId: string) => {
  await db.delete(run).where(eq(run.id, runId));
};

export const updateRunStatus = async (runId: string, status: (typeof runStatusEnum)[number], timestamp?: Date) => {
  const updates: Partial<UpdateRun> = { status };

  if (status === "running" && timestamp) {
    updates.startedAt = timestamp;
  } else if ((status === "completed" || status === "failed" || status === "cancelled") && timestamp) {
    updates.completedAt = timestamp;
  }

  return updateRun(runId, updates);
};
