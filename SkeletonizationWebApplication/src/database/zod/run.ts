/**
 * @file run.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Defines Zod schemas and types for run configuration and entities.
 * @description Provides run CRUD schemas plus runtime configuration schemas for skeletonization run creation.
 * @version 1.0
 * @date 2026-03-16
 */

import { createInsertSchema, createSelectSchema } from "drizzle-zod";
import { z } from "zod";

import { ALGORITHMS } from "@/algorithms";

import { run } from "../schema";
import { DEFAULT_OUTPUT_FORMAT_ENUM } from "../schema/preferences";

/** @brief Insert schema for run rows with name validation. */
export const insertRunSchema = createInsertSchema(run, {
  name: (schema) => schema.trim().min(1, "Run name is required").max(50, "Run name is too long")
});

/** @brief Select schema for run rows. */
export const selectRunSchema = createSelectSchema(run);

/** @brief Partial update schema for run rows requiring `id`. */
export const updateRunSchema = insertRunSchema.partial().required({ id: true });

/** @brief Insert-run type inferred from schema. */
export type InsertRun = z.infer<typeof insertRunSchema>;
/** @brief Select-run type inferred from schema. */
export type SelectRun = z.infer<typeof selectRunSchema>;
/** @brief Update-run type inferred from schema. */
export type UpdateRun = z.infer<typeof updateRunSchema>;

/** @brief Schema for interactive run configuration form. */
export const runConfigurationSchema = insertRunSchema.pick({ name: true }).extend({
  algorithms: z.array(z.enum(ALGORITHMS)).min(1, "Please select at least one algorithm"),
  imageIds: z.array(z.string()).min(1, "Please select at least one image")
});

/** @brief Run configuration type inferred from schema. */
export type RunConfiguration = z.infer<typeof runConfigurationSchema>;

/** @brief Schema for skeletonization execution parameters. */
export const skeletonizationParamsSchema = z.object({
  preprocessAllImages: z.boolean().default(true),
  preprocessByImageId: z.record(z.uuid(), z.boolean()).default({}),
  outputFormat: z.enum(DEFAULT_OUTPUT_FORMAT_ENUM).default("PNG")
});

/** @brief Schema for creating a skeletonization run request payload. */
export const createSkeletonizationRunSchema = runConfigurationSchema
  .extend({
    imageIds: z.array(z.uuid()).min(1, "At least one image must be selected"),
    params: skeletonizationParamsSchema.default({
      preprocessAllImages: true,
      preprocessByImageId: {},
      outputFormat: "PNG"
    })
  })
  .refine((data) => data.algorithms.length > 0, {
    message: "At least one algorithm must be selected",
    path: ["algorithms"]
  });

/** @brief Create-skeletonization-run payload type inferred from schema. */
export type CreateSkeletonizationRun = z.infer<typeof createSkeletonizationRunSchema>;

/** @brief Union type of available run status values. */
export type RunStatus = z.infer<typeof selectRunSchema.shape.status>;
