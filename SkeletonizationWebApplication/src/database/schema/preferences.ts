import { boolean, pgTable, text } from "drizzle-orm/pg-core";

import { user } from "./auth";

export const themeEnum = ["system", "light", "dark"] as const;
export const defaultOutputFormatEnum = ["PNG", "JPEG", "TIFF", "BMP"] as const;

type Theme = (typeof themeEnum)[number];

type DefaultOutputFormat = (typeof defaultOutputFormatEnum)[number];

export const defaultPreferences = {
  theme: "system" as Theme,
  compactMode: false,
  pushNotifications: true,
  timezone: "UTC",
  autoSaveResults: true,
  defaultOutputFormat: "PNG" as DefaultOutputFormat
} as const;

export const userPreferences = pgTable("user_preferences", {
  userId: text("user_id")
    .primaryKey()
    .notNull()
    .references(() => user.id, { onDelete: "cascade" }),

  // Appearance
  theme: text("theme", { enum: themeEnum }).default(defaultPreferences.theme).notNull(),
  compactMode: boolean("compact_mode").default(defaultPreferences.compactMode).notNull(),

  // Notifications
  pushNotifications: boolean("push_notifications").default(defaultPreferences.pushNotifications).notNull(),
  // Timezone
  timezone: text("timezone").default(defaultPreferences.timezone).notNull(),

  // Processing Preferences
  autoSaveResults: boolean("auto_save_results").default(defaultPreferences.autoSaveResults).notNull(),
  defaultOutputFormat: text("default_output_format", { enum: defaultOutputFormatEnum })
    .default(defaultPreferences.defaultOutputFormat)
    .notNull()
});
