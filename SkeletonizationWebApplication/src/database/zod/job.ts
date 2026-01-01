import { createInsertSchema, createSelectSchema } from "drizzle-zod";
import { z } from "zod";

import { job } from "../schema";

export const insertJobSchema = createInsertSchema(job, {
  params: z.any().optional()
});

export const selectJobSchema = createSelectSchema(job);

export const updateJobSchema = insertJobSchema.partial().required({ id: true });

export type InsertJob = z.infer<typeof insertJobSchema>;
export type SelectJob = z.infer<typeof selectJobSchema>;
export type UpdateJob = z.infer<typeof updateJobSchema>;

export type JobStatus = z.infer<typeof selectJobSchema.shape.status>;
