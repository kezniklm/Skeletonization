import { createInsertSchema, createSelectSchema } from "drizzle-zod";
import { type z } from "zod";

import { jobStats } from "../schema";

export const insertJobStatsSchema = createInsertSchema(jobStats);
export const selectJobStatsSchema = createSelectSchema(jobStats);

export const updateJobStatsSchema = insertJobStatsSchema.partial().required({ jobId: true });

export type InsertJobStats = z.infer<typeof insertJobStatsSchema>;
export type SelectJobStats = z.infer<typeof selectJobStatsSchema>;
export type UpdateJobStats = z.infer<typeof updateJobStatsSchema>;
