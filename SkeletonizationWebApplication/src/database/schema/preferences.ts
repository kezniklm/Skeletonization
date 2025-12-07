import { boolean, pgTable, text } from "drizzle-orm/pg-core";

import { user } from "./auth";

export const themeEnum = ["system", "light", "dark"] as const;
export const defaultOutputFormatEnum = ["PNG", "JPEG", "TIFF", "BMP"] as const;

export const userPreferences = pgTable("user_preferences", {
  userId: text("user_id")
    .primaryKey()
    .notNull()
    .references(() => user.id, { onDelete: "cascade" }),

  // Appearance
  theme: text("theme", { enum: themeEnum }).default("system").notNull(),
  compactMode: boolean("compact_mode").default(false).notNull(),

  // Notifications
  pushNotifications: boolean("push_notifications").default(false).notNull(),

  // Timezone
  timezone: text("timezone").default("UTC").notNull(),

  // Processing Preferences
  autoSaveResults: boolean("auto_save_results").default(true).notNull(),
  defaultOutputFormat: text("default_output_format", { enum: defaultOutputFormatEnum }).default("PNG").notNull()
});
