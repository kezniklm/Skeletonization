/**
 * @file job-stats.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Defines Zod schemas and types for job statistics.
 * @description Provides insert/select/update schema derivations and worker type aliases for job stats entities.
 * @version 1.0
 * @date 2026-03-16
 */

import { createInsertSchema, createSelectSchema } from "drizzle-zod";
import { type z } from "zod";

import { jobStats } from "../schema";

/** @brief Insert schema for job stats rows. */
export const insertJobStatsSchema = createInsertSchema(jobStats);
/** @brief Select schema for job stats rows. */
export const selectJobStatsSchema = createSelectSchema(jobStats);

/** @brief Partial update schema for job stats rows requiring `jobId`. */
export const updateJobStatsSchema = insertJobStatsSchema.partial().required({ jobId: true });

/** @brief Insert-job-stats type inferred from schema. */
export type InsertJobStats = z.infer<typeof insertJobStatsSchema>;
/** @brief Select-job-stats type inferred from schema. */
export type SelectJobStats = z.infer<typeof selectJobStatsSchema>;
/** @brief Update-job-stats type inferred from schema. */
export type UpdateJobStats = z.infer<typeof updateJobStatsSchema>;

/** @brief Union type of available worker runtime values. */
export type WorkerType = z.infer<typeof selectJobStatsSchema.shape.workerType>;
