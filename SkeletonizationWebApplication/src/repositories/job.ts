import { and, asc, desc, eq } from "drizzle-orm";

import { db } from "@/database";
import { job, type JobStatus } from "@/database/schema/job";
import { type InsertJob, type UpdateJob } from "@/database/zod/job";

export const getJobById = async (jobId: string) => {
  const [result] = await db.select().from(job).where(eq(job.id, jobId));

  return result ?? null;
};

export const getJobsByRunId = async (runId: string) =>
  db.select().from(job).where(eq(job.runId, runId)).orderBy(asc(job.ordinal));

export const getJobsByImageId = async (imageId: string) =>
  db.select().from(job).where(eq(job.imageId, imageId)).orderBy(desc(job.createdAt));

export const getJobsByStatus = async (status: JobStatus) =>
  db.select().from(job).where(eq(job.status, status)).orderBy(desc(job.createdAt));

export const getJobByRunAndImage = async (runId: string, imageId: string) => {
  const [result] = await db
    .select()
    .from(job)
    .where(and(eq(job.runId, runId), eq(job.imageId, imageId)));

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

export const updateJobStatus = async (jobId: string, status: JobStatus) => updateJob(jobId, { status });
