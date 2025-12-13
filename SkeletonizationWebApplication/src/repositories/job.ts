import { desc, eq } from "drizzle-orm";

import { db } from "@/database";
import { job, type jobStatusEnum } from "@/database/schema/job";
import { type InsertJob, type UpdateJob } from "@/database/zod/job";

export const getJobById = async (jobId: string) => {
  const [result] = await db.select().from(job).where(eq(job.id, jobId));

  return result ?? null;
};

export const getJobsByRunImageId = async (runImageId: string) =>
  db.select().from(job).where(eq(job.runImageId, runImageId)).orderBy(desc(job.createdAt));

export const getJobsByStatus = async (status: (typeof jobStatusEnum)[number]) =>
  db.select().from(job).where(eq(job.status, status)).orderBy(desc(job.createdAt));

export const getJobsByWorkerId = async (workerId: string) =>
  db.select().from(job).where(eq(job.workerId, workerId)).orderBy(desc(job.createdAt));

export const getLatestJobForRunImage = async (runImageId: string) => {
  const [result] = await db.select().from(job).where(eq(job.runImageId, runImageId)).orderBy(desc(job.createdAt));

  return result ?? null;
};

export const createJob = async (jobData: InsertJob) => {
  const [newJob] = await db.insert(job).values(jobData).returning();

  return newJob;
};

export const createJobsBulk = async (jobsData: InsertJob[]) => {
  if (jobsData.length === 0) return [];
  const newJobs = await db.insert(job).values(jobsData).returning();
  return newJobs;
};

export const updateJob = async (jobId: string, jobData: Partial<UpdateJob>) => {
  const [updated] = await db.update(job).set(jobData).where(eq(job.id, jobId)).returning();

  return updated;
};

export const deleteJob = async (jobId: string) => {
  await db.delete(job).where(eq(job.id, jobId));
};

export const updateJobStatus = async (jobId: string, status: (typeof jobStatusEnum)[number], timestamp?: Date) => {
  const updates: Partial<UpdateJob> = { status };

  if (status === "processing" && timestamp) {
    updates.startedAt = timestamp;
  } else if ((status === "completed" || status === "failed" || status === "cancelled") && timestamp) {
    updates.finishedAt = timestamp;
  }

  return updateJob(jobId, updates);
};

export const incrementJobAttempt = async (jobId: string) => {
  const current = await getJobById(jobId);
  if (!current) return null;

  return updateJob(jobId, { attempts: (current.attempts ?? 0) + 1 });
};

export const appendJobLogs = async (jobId: string, newLogs: string) => {
  const current = await getJobById(jobId);
  if (!current) return null;

  const currentLogs = current.logs ?? "";
  const updatedLogs = currentLogs ? `${currentLogs}\n${newLogs}` : newLogs;

  return updateJob(jobId, { logs: updatedLogs });
};
