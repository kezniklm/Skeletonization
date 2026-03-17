/**
 * @file job-stats.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Provides repository helpers for job statistics.
 * @description Supports retrieval and upsert behavior for processing metrics associated with skeletonization jobs.
 * @version 1.0
 * @date 2026-03-16
 */

import { eq } from "drizzle-orm";

import { db } from "@/database";
import { jobStats } from "@/database/schema/job-stats";
import { insertJobStatsSchema, type UpdateJobStats } from "@/database/zod/job-stats";

/** @brief Returns stats row for a job id. */
export const getJobStatsByJobId = async (jobId: string) => {
  const [result] = await db.select().from(jobStats).where(eq(jobStats.jobId, jobId));
  return result ?? null;
};

/** @brief Inserts or updates job stats record for a job id. */
export const upsertJobStats = async (jobId: string, stats: Partial<Omit<UpdateJobStats, "jobId">>) => {
  const existing = await getJobStatsByJobId(jobId);

  if (!existing) {
    const insertValues = insertJobStatsSchema.parse({ jobId, ...stats });
    const [created] = await db.insert(jobStats).values(insertValues).returning();
    return created;
  }

  const [updated] = await db.update(jobStats).set(stats).where(eq(jobStats.jobId, jobId)).returning();

  return updated ?? null;
};
