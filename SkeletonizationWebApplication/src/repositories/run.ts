import { and, asc, desc, eq } from "drizzle-orm";
import { alias } from "drizzle-orm/pg-core";

import { type Algorithm } from "@/algorithms";
import { db } from "@/database";
import { image } from "@/database/schema/image";
import { job } from "@/database/schema/job";
import { jobStats } from "@/database/schema/job-stats";
import { run } from "@/database/schema/run";
import { type InsertRun, type RunStatus, type UpdateRun } from "@/database/zod/run";

export type LabJob = {
  id: string;
  ordinal: number;
  status: string;
  algorithm: Algorithm;
  processingTimeMs: number;
  image: {
    id: string;
    name: string;
    storagePath: string;
  };
  producedImage: null | {
    id: string;
    storagePath: string;
    name: string;
    status: string;
  };
};

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

export const getRunById = async (runId: string) => {
  const [result] = await db.select().from(run).where(eq(run.id, runId));

  return result ?? null;
};

export const getRunsByUserId = async (userId: string) =>
  db.select().from(run).where(eq(run.userId, userId)).orderBy(desc(run.createdAt));

export const getRunCountByUserId = async (userId: string) => db.$count(run, eq(run.userId, userId));

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
        processingTimeMs: jobStats.processingTimeMs
      },
      inputImage: {
        id: inputImage.id,
        name: inputImage.originalFilename,
        storagePath: inputImage.storagePath
      },
      producedImage: {
        id: producedImage.id,
        name: producedImage.originalFilename,
        storagePath: producedImage.storagePath,
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
        image: {
          id: row.inputImage.id,
          name: row.inputImage.name ?? "",
          storagePath: row.inputImage.storagePath ?? ""
        },
        producedImage: null
      };
      runEntry.jobs.push(jobEntry);
      runEntry.jobMap.set(row.job.id, jobEntry);
    }

    if (row.producedImage?.id && !jobEntry.producedImage) {
      jobEntry.producedImage = {
        id: row.producedImage.id,
        storagePath: row.producedImage.storagePath ?? "",
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

export const getRunsByStatus = async (status: RunStatus) =>
  db.select().from(run).where(eq(run.status, status)).orderBy(desc(run.createdAt));

export const getUserRunsByStatus = async (userId: string, status: RunStatus) =>
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

export const updateRunStatus = async (runId: string, status: RunStatus, timestamp?: Date) => {
  const updates: Partial<UpdateRun> = { status };

  if (status === "running" && timestamp) {
    updates.startedAt = timestamp;
  } else if ((status === "completed" || status === "failed" || status === "cancelled") && timestamp) {
    updates.completedAt = timestamp;
  }

  return updateRun(runId, updates);
};

export const getRunStatus = async (runId: string): Promise<RunStatus | null> => {
  const [runData] = await db.select({ status: run.status }).from(run).where(eq(run.id, runId));
  return runData?.status ?? null;
};

export const getRunOwnerAndName = async (runId: string): Promise<{ userId: string; name: string | null } | null> => {
  const [runData] = await db.select({ userId: run.userId, name: run.name }).from(run).where(eq(run.id, runId));
  return runData ?? null;
};
