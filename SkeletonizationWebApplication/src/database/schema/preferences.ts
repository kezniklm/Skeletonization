import { boolean, pgTable, text } from "drizzle-orm/pg-core";

import { user } from "./auth";

export const DEFAULT_OUTPUT_FORMAT_ENUM = ["BMP", "JPEG", "PNG", "TIFF"] as const;

const THEME_ENUM = ["system", "light", "dark"] as const;

type DefaultOutputFormat = (typeof DEFAULT_OUTPUT_FORMAT_ENUM)[number];
type Theme = (typeof THEME_ENUM)[number];

export const defaultPreferences = {
  theme: "system" satisfies Theme,
  compactMode: false,
  pushNotifications: true,
  timezone: "UTC",
  defaultOutputFormat: "PNG" satisfies DefaultOutputFormat
} as const;

export const userPreferences = pgTable("user_preferences", {
  userId: text("user_id")
    .primaryKey()
    .notNull()
    .references(() => user.id, { onDelete: "cascade" }),

  // Appearance
  theme: text("theme", { enum: THEME_ENUM }).default(defaultPreferences.theme).notNull(),
  compactMode: boolean("compact_mode").default(defaultPreferences.compactMode).notNull(),

  // Notifications
  pushNotifications: boolean("push_notifications").default(defaultPreferences.pushNotifications).notNull(),

  // Timezone
  timezone: text("timezone").default(defaultPreferences.timezone).notNull(),

  // Processing Preferences
  defaultOutputFormat: text("default_output_format", { enum: DEFAULT_OUTPUT_FORMAT_ENUM })
    .default(defaultPreferences.defaultOutputFormat)
    .notNull()
});
