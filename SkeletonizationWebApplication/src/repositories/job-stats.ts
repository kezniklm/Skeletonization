import { eq } from "drizzle-orm";

import { db } from "@/database";
import { jobStats } from "@/database/schema/job-stats";
import { insertJobStatsSchema, type UpdateJobStats } from "@/database/zod/job-stats";

export const getJobStatsByJobId = async (jobId: string) => {
  const [result] = await db.select().from(jobStats).where(eq(jobStats.jobId, jobId));
  return result ?? null;
};

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
