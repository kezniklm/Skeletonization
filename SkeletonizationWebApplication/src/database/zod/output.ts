import { createInsertSchema, createSelectSchema } from "drizzle-zod";
import { type z } from "zod";

import { output } from "../schema";

export const insertOutputSchema = createInsertSchema(output, {
  storagePath: (schema) => schema.min(1, "Storage path is required"),
  url: (schema) => schema.url("Invalid URL"),
  mime: (schema) => schema.min(1, "MIME type is required"),
  sizeBytes: (schema) => schema.positive("Size must be positive")
});

export const selectOutputSchema = createSelectSchema(output);

export const updateOutputSchema = insertOutputSchema.partial().required({ id: true });

export type InsertOutput = z.infer<typeof insertOutputSchema>;
export type SelectOutput = z.infer<typeof selectOutputSchema>;
export type UpdateOutput = z.infer<typeof updateOutputSchema>;
