import { createInsertSchema, createSelectSchema } from "drizzle-zod";
import { type z } from "zod";

import { run } from "../schema";

export const insertRunSchema = createInsertSchema(run, {
  name: (schema) => schema.min(1, "Run name is required")
});

export const selectRunSchema = createSelectSchema(run);

export const updateRunSchema = insertRunSchema.partial().required({ id: true });

export type InsertRun = z.infer<typeof insertRunSchema>;
export type SelectRun = z.infer<typeof selectRunSchema>;
export type UpdateRun = z.infer<typeof updateRunSchema>;
