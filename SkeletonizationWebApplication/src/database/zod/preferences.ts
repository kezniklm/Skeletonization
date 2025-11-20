import { createInsertSchema } from "drizzle-zod";
import { type z } from "zod";

import { userPreferences } from "../schema/preferences";

export const userPreferencesSchema = createInsertSchema(userPreferences).omit({ userId: true });

export type UserPreferences = z.infer<typeof userPreferencesSchema>;

export const defaultPreferences: UserPreferences = {
  theme: "system",
  compactMode: false,
  emailNotifications: true,
  pushNotifications: false,
  timezone: "UTC",
  autoSaveResults: true,
  defaultOutputFormat: "PNG"
};
