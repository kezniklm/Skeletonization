import { createInsertSchema } from "drizzle-zod";
import { type z } from "zod";

import { userPreferences } from "../schema/preferences";

export const userPreferencesSchema = createInsertSchema(userPreferences).omit({ userId: true }).required();

export type UserPreferences = z.infer<typeof userPreferencesSchema>;

export type FileOutputFormat = z.infer<typeof userPreferencesSchema.shape.defaultOutputFormat>;

export type ImageFormat = Lowercase<FileOutputFormat>;

export type Theme = z.infer<typeof userPreferencesSchema.shape.theme>;
