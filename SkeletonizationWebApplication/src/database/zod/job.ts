/**
 * @file job.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Defines Zod schemas and types for job rows.
 * @description Provides insert/select/update schema derivations and job status aliases.
 * @version 1.0
 * @date 2026-03-16
 */

import { createInsertSchema, createSelectSchema } from "drizzle-zod";
import { z } from "zod";

import { job } from "../schema";

/** @brief Insert schema for job rows. */
export const insertJobSchema = createInsertSchema(job, {
  params: z.any().optional()
});

/** @brief Select schema for job rows. */
export const selectJobSchema = createSelectSchema(job);

/** @brief Partial update schema for job rows requiring `id`. */
export const updateJobSchema = insertJobSchema.partial().required({ id: true });

/** @brief Insert-job type inferred from schema. */
export type InsertJob = z.infer<typeof insertJobSchema>;
/** @brief Select-job type inferred from schema. */
export type SelectJob = z.infer<typeof selectJobSchema>;
/** @brief Update-job type inferred from schema. */
export type UpdateJob = z.infer<typeof updateJobSchema>;

/** @brief Union type of available job status values. */
export type JobStatus = z.infer<typeof selectJobSchema.shape.status>;
