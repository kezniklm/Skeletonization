import { createInsertSchema } from "drizzle-zod";
import { type z } from "zod";

import { userPreferences } from "../schema/preferences";

export const userPreferencesSchema = createInsertSchema(userPreferences).omit({ userId: true }).required();

export type UserPreferences = z.infer<typeof userPreferencesSchema>;

export type FileOutputFormat = z.infer<typeof userPreferencesSchema.shape.defaultOutputFormat>;

export const defaultPreferences: UserPreferences = {
  theme: "system",
  compactMode: false,
  pushNotifications: true,
  timezone: "UTC",
  autoSaveResults: true,
  defaultOutputFormat: "PNG"
};
