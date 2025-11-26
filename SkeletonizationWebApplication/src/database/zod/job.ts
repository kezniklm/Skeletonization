import { createInsertSchema, createSelectSchema } from "drizzle-zod";
import { type z } from "zod";

import { job } from "../schema";

export const insertJobSchema = createInsertSchema(job, {
  attempts: (schema) => schema.nonnegative("Attempts must be non-negative")
});

export const selectJobSchema = createSelectSchema(job);

export const updateJobSchema = insertJobSchema.partial().required({ id: true });

export type InsertJob = z.infer<typeof insertJobSchema>;
export type SelectJob = z.infer<typeof selectJobSchema>;
export type UpdateJob = z.infer<typeof updateJobSchema>;
