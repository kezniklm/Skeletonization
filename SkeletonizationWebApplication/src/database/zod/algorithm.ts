import { createInsertSchema, createSelectSchema } from "drizzle-zod";
import { type z } from "zod";

import { algorithm } from "../schema";

export const insertAlgorithmSchema = createInsertSchema(algorithm, {
  name: (schema) => schema.min(1, "Algorithm name is required")
});

export const selectAlgorithmSchema = createSelectSchema(algorithm);

export const updateAlgorithmSchema = insertAlgorithmSchema.partial().required({ id: true });

export type InsertAlgorithm = z.infer<typeof insertAlgorithmSchema>;
export type SelectAlgorithm = z.infer<typeof selectAlgorithmSchema>;
export type UpdateAlgorithm = z.infer<typeof updateAlgorithmSchema>;
