import { createInsertSchema, createSelectSchema } from "drizzle-zod";
import { z } from "zod";

import { ALGORITHMS } from "@/algorithms";

import { run } from "../schema";
import { DEFAULT_OUTPUT_FORMAT_ENUM } from "../schema/preferences";

export const insertRunSchema = createInsertSchema(run, {
  name: (schema) => schema.trim().min(1, "Run name is required").max(50, "Run name is too long")
});

export const selectRunSchema = createSelectSchema(run);

export const updateRunSchema = insertRunSchema.partial().required({ id: true });

export type InsertRun = z.infer<typeof insertRunSchema>;
export type SelectRun = z.infer<typeof selectRunSchema>;
export type UpdateRun = z.infer<typeof updateRunSchema>;

export const runConfigurationSchema = insertRunSchema.pick({ name: true }).extend({
  algorithms: z.array(z.enum(ALGORITHMS)).min(1, "Please select at least one algorithm"),
  imageIds: z.array(z.string()).min(1, "Please select at least one image")
});

export type RunConfiguration = z.infer<typeof runConfigurationSchema>;

export const skeletonizationParamsSchema = z.object({
  preprocessAllImages: z.boolean().default(true),
  preprocessByImageId: z.record(z.uuid(), z.boolean()).default({}),
  outputFormat: z.enum(DEFAULT_OUTPUT_FORMAT_ENUM).default("PNG")
});

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

export type CreateSkeletonizationRun = z.infer<typeof createSkeletonizationRunSchema>;

export type RunStatus = z.infer<typeof selectRunSchema.shape.status>;
