/**
 * @file job.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Provides repository helpers for job records.
 * @description Contains query and mutation operations for skeletonization jobs and ownership lookup.
 * @version 1.0
 * @date 2026-03-16
 */

import { and, asc, desc, eq } from "drizzle-orm";

import { db } from "@/database";
import { image } from "@/database/schema/image";
import { job } from "@/database/schema/job";
import { type InsertJob, type JobStatus, type UpdateJob } from "@/database/zod/job";

/**
 * @brief Represents a job enriched with owning user reference.
 */
export type JobWithOwner = {
  jobId: string;
  runId: string;
  inputImageId: string | null;
  userId: string | null;
};

/** @brief Returns a job by id. */
export const getJobById = async (jobId: string) => {
  const [result] = await db.select().from(job).where(eq(job.id, jobId));

  return result ?? null;
};

/** @brief Returns job id, run id, input image id, and owner id details. */
export const getJobWithOwner = async (jobId: string): Promise<JobWithOwner | null> => {
  const [jobData] = await db
    .select({
      jobId: job.id,
      runId: job.runId,
      inputImageId: job.imageId,
      userId: image.userId
    })
    .from(job)
    .leftJoin(image, eq(job.imageId, image.id))
    .where(eq(job.id, jobId));

  return jobData ?? null;
};

/** @brief Lists jobs for a run ordered by ordinal. */
export const getJobsByRunId = async (runId: string) =>
  db.select().from(job).where(eq(job.runId, runId)).orderBy(asc(job.ordinal));

/** @brief Lists jobs generated for an image ordered by creation date descending. */
export const getJobsByImageId = async (imageId: string) =>
  db.select().from(job).where(eq(job.imageId, imageId)).orderBy(desc(job.createdAt));

/** @brief Lists jobs by job status. */
export const getJobsByStatus = async (status: JobStatus) =>
  db.select().from(job).where(eq(job.status, status)).orderBy(desc(job.createdAt));

/** @brief Returns a job by run id and image id combination. */
export const getJobByRunAndImage = async (runId: string, imageId: string) => {
  const [result] = await db
    .select()
    .from(job)
    .where(and(eq(job.runId, runId), eq(job.imageId, imageId)));

  return result ?? null;
};

/** @brief Inserts a new job row. */
export const createJob = async (jobData: InsertJob) => {
  const [newJob] = await db.insert(job).values(jobData).returning();

  return newJob;
};

/** @brief Inserts multiple jobs in a single operation. */
export const createJobsBulk = async (jobsData: InsertJob[]) => {
  if (jobsData.length === 0) return [];
  const newJobs = await db.insert(job).values(jobsData).returning();
  return newJobs;
};

/** @brief Updates a job row by id. */
export const updateJob = async (jobId: string, jobData: Partial<UpdateJob>) => {
  const [updated] = await db.update(job).set(jobData).where(eq(job.id, jobId)).returning();

  return updated;
};

/** @brief Deletes a job by id. */
export const deleteJob = async (jobId: string) => {
  await db.delete(job).where(eq(job.id, jobId));
};

/** @brief Updates only the status column of a job. */
export const updateJobStatus = async (jobId: string, status: JobStatus) => updateJob(jobId, { status });
