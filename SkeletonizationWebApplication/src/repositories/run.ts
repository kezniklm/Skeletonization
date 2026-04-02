/**
 * @file run.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Provides repository helpers for run entities.
 * @description Contains CRUD, status transitions, and lab-detail aggregation queries for skeletonization runs.
 * @version 1.0
 * @date 2026-03-16
 */

import { and, asc, desc, eq, ilike, notInArray } from "drizzle-orm";
import { alias } from "drizzle-orm/pg-core";

import { type Algorithm } from "@/algorithms";
import { db } from "@/database";
import { image } from "@/database/schema/image";
import { job } from "@/database/schema/job";
import { jobStats } from "@/database/schema/job-stats";
import { run } from "@/database/schema/run";
import { type WorkerType } from "@/database/zod/job-stats";
import { type InsertRun, type RunStatus, type UpdateRun } from "@/database/zod/run";

/**
 * @brief Represents a job entry in lab run detail view.
 */
export type LabJob = {
  id: string;
  ordinal: number;
  status: string;
  algorithm: Algorithm;
  processingTimeMs: number;
  workerId: string;
  workerType: WorkerType;
  image: {
    id: string;
    name: string;
    url: string;
  };
  producedImage: null | {
    id: string;
    url: string;
    name: string;
    status: string;
  };
};

/**
 * @brief Represents a run entry with nested jobs for lab view.
 */
export type LabRun = {
  id: string;
  name: string | null;
  status: string;
  createdAt: string | null;
  startedAt: string | null;
  completedAt: string | null;
  jobs: LabJob[];
};

const toIsoOrNull = (value: Date | null | undefined) => (value ? value.toISOString() : null);

/** @brief Returns a run by id. */
export const getRunById = async (runId: string) => {
  const [result] = await db.select().from(run).where(eq(run.id, runId));

  return result ?? null;
};

/** @brief Lists runs for a user ordered by creation date descending. */
export const getRunsByUserId = async (userId: string) =>
  db.select().from(run).where(eq(run.userId, userId)).orderBy(desc(run.createdAt));

/** @brief Returns true when a user already has a run with the same name (case-insensitive). */
export const runNameExistsForUser = async (userId: string, name: string): Promise<boolean> => {
  const trimmedName = name.trim();

  if (!trimmedName) {
    return false;
  }

  // Escape LIKE wildcards so ilike performs exact-name matching, just case-insensitive.
  const escapedName = trimmedName.replace(/([%_\\])/g, "\\$1");

  const existingRuns = await db
    .select({ id: run.id })
    .from(run)
    .where(and(eq(run.userId, userId), ilike(run.name, escapedName)))
    .limit(1);

  return existingRuns.length > 0;
};

/** @brief Counts runs for a user. */
export const getRunCountByUserId = async (userId: string) => db.$count(run, eq(run.userId, userId));

/** @brief Returns runs with nested job and image details for lab display. */
export const getRunsWithDetailsByUserId = async (userId: string): Promise<LabRun[]> => {
  const inputImage = alias(image, "input_image");
  const producedImage = alias(image, "produced_image");

  const rows = await db
    .select({
      run: {
        id: run.id,
        name: run.name,
        status: run.status,
        createdAt: run.createdAt,
        startedAt: run.startedAt,
        completedAt: run.completedAt
      },
      job: {
        id: job.id,
        ordinal: job.ordinal,
        status: job.status,
        algorithm: job.algorithm
      },
      jobStats: {
        processingTimeMs: jobStats.processingTimeMs,
        workerId: jobStats.workerId,
        workerType: jobStats.workerType
      },
      inputImage: {
        id: inputImage.id,
        name: inputImage.originalFilename,
        url: inputImage.url
      },
      producedImage: {
        id: producedImage.id,
        name: producedImage.originalFilename,
        url: producedImage.url,
        status: producedImage.status
      }
    })
    .from(run)
    .where(eq(run.userId, userId))
    .leftJoin(job, eq(job.runId, run.id))
    .leftJoin(jobStats, eq(jobStats.jobId, job.id))
    .leftJoin(inputImage, eq(job.imageId, inputImage.id))
    .leftJoin(producedImage, eq(producedImage.generatedByJobId, job.id))
    .orderBy(desc(run.createdAt), asc(job.ordinal));

  const runMap = new Map<string, LabRun & { jobMap: Map<string, LabJob> }>();

  for (const row of rows) {
    let runEntry = runMap.get(row.run.id);
    if (!runEntry) {
      runEntry = {
        id: row.run.id,
        name: row.run.name,
        status: row.run.status,
        createdAt: toIsoOrNull(row.run.createdAt),
        startedAt: toIsoOrNull(row.run.startedAt),
        completedAt: toIsoOrNull(row.run.completedAt),
        jobs: [],
        jobMap: new Map()
      };
      runMap.set(row.run.id, runEntry);
    }

    if (!row.job?.id || !row.inputImage?.id) continue;

    let jobEntry = runEntry.jobMap.get(row.job.id);
    if (!jobEntry) {
      jobEntry = {
        id: row.job.id,
        ordinal: row.job.ordinal ?? 0,
        status: row.job.status ?? "pending",
        algorithm: row.job.algorithm as Algorithm,
        processingTimeMs: row.jobStats?.processingTimeMs ?? 0,
        workerId: row.jobStats?.workerId ?? "unknown",
        workerType: row.jobStats?.workerType ?? "cpu",
        image: {
          id: row.inputImage.id,
          name: row.inputImage.name ?? "",
          url: row.inputImage.url ?? ""
        },
        producedImage: null
      };
      runEntry.jobs.push(jobEntry);
      runEntry.jobMap.set(row.job.id, jobEntry);
    }

    if (row.producedImage?.id && !jobEntry.producedImage) {
      jobEntry.producedImage = {
        id: row.producedImage.id,
        url: row.producedImage.url ?? "",
        name: row.producedImage.name ?? "",
        status: row.producedImage.status ?? ""
      };
    }
  }

  return Array.from(runMap.values()).map((entry) => ({
    id: entry.id,
    name: entry.name,
    status: entry.status,
    createdAt: entry.createdAt,
    startedAt: entry.startedAt,
    completedAt: entry.completedAt,
    jobs: entry.jobs
  }));
};

/** @brief Lists runs by status. */
export const getRunsByStatus = async (status: RunStatus) =>
  db.select().from(run).where(eq(run.status, status)).orderBy(desc(run.createdAt));

/** @brief Lists runs for a user filtered by status. */
export const getUserRunsByStatus = async (userId: string, status: RunStatus) =>
  db
    .select()
    .from(run)
    .where(and(eq(run.userId, userId), eq(run.status, status)))
    .orderBy(desc(run.createdAt));

/** @brief Inserts a new run. */
export const createRun = async (runData: InsertRun) => {
  const [newRun] = await db.insert(run).values(runData).returning();

  return newRun;
};

/** @brief Updates run fields by id. */
export const updateRun = async (runId: string, runData: Partial<UpdateRun>) => {
  const [updated] = await db.update(run).set(runData).where(eq(run.id, runId)).returning();

  return updated;
};

/** @brief Deletes a run by id. */
export const deleteRun = async (runId: string) => {
  await db.delete(run).where(eq(run.id, runId));
};

/** @brief Updates run status and relevant timestamp fields, guarding terminal transitions. */
export const updateRunStatus = async (runId: string, status: RunStatus, timestamp?: Date) => {
  const updates: Partial<UpdateRun> = { status };
  const terminalStatuses: RunStatus[] = ["completed", "failed", "cancelled"];

  if (status === "running" && timestamp) {
    updates.startedAt = timestamp;
  } else if ((status === "completed" || status === "failed" || status === "cancelled") && timestamp) {
    updates.completedAt = timestamp;
  }

  if (terminalStatuses.includes(status)) {
    const [updated] = await db
      .update(run)
      .set(updates)
      .where(and(eq(run.id, runId), notInArray(run.status, terminalStatuses)))
      .returning();

    return updated ?? null;
  }

  return updateRun(runId, updates);
};

/** @brief Returns current status of a run. */
export const getRunStatus = async (runId: string): Promise<RunStatus | null> => {
  const [runData] = await db.select({ status: run.status }).from(run).where(eq(run.id, runId));
  return runData?.status ?? null;
};

/** @brief Returns owning user id and name metadata for a run. */
export const getRunOwnerAndName = async (runId: string): Promise<{ userId: string; name: string | null } | null> => {
  const [runData] = await db.select({ userId: run.userId, name: run.name }).from(run).where(eq(run.id, runId));
  return runData ?? null;
};
