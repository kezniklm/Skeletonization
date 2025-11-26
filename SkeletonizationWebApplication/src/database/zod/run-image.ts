import { createInsertSchema, createSelectSchema } from "drizzle-zod";
import { type z } from "zod";

import { runImage } from "../schema";

export const insertRunImageSchema = createInsertSchema(runImage, {
  ordinal: (schema) => schema.nonnegative("Ordinal must be non-negative"),
  attempt: (schema) => schema.nonnegative("Attempt must be non-negative")
});

export const selectRunImageSchema = createSelectSchema(runImage);

export const updateRunImageSchema = insertRunImageSchema.partial().required({ id: true });

export type InsertRunImage = z.infer<typeof insertRunImageSchema>;
export type SelectRunImage = z.infer<typeof selectRunImageSchema>;
export type UpdateRunImage = z.infer<typeof updateRunImageSchema>;
