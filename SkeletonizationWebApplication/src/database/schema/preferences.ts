/**
 * @file preferences.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Defines user preferences table and defaults.
 * @description Declares preference enums, default values, and persisted preference schema for appearance, notifications, timezone, and output format.
 * @version 1.0
 * @date 2026-03-16
 */

import { boolean, pgTable, text } from "drizzle-orm/pg-core";

import { user } from "./auth";

/** @brief Supported output format enum values. */
export const DEFAULT_OUTPUT_FORMAT_ENUM = ["BMP", "JPEG", "PNG", "TIFF"] as const;

const THEME_ENUM = ["system", "light", "dark"] as const;

type DefaultOutputFormat = (typeof DEFAULT_OUTPUT_FORMAT_ENUM)[number];
type Theme = (typeof THEME_ENUM)[number];

/** @brief Application default user preference values. */
export const defaultPreferences = {
  theme: "system" satisfies Theme,
  animatedBackgroundDisabled: false,
  pushNotifications: true,
  timezone: "UTC",
  defaultOutputFormat: "PNG" satisfies DefaultOutputFormat
} as const;

/** @brief Database table schema for user preferences. */
export const userPreferences = pgTable("user_preferences", {
  userId: text("user_id")
    .primaryKey()
    .notNull()
    .references(() => user.id, { onDelete: "cascade" }),

  // Appearance
  theme: text("theme", { enum: THEME_ENUM }).default(defaultPreferences.theme).notNull(),
  animatedBackgroundDisabled: boolean("animated_background_disabled")
    .default(defaultPreferences.animatedBackgroundDisabled)
    .notNull(),

  // Notifications
  pushNotifications: boolean("push_notifications").default(defaultPreferences.pushNotifications).notNull(),

  // Timezone
  timezone: text("timezone").default(defaultPreferences.timezone).notNull(),

  // Processing Preferences
  defaultOutputFormat: text("default_output_format", { enum: DEFAULT_OUTPUT_FORMAT_ENUM })
    .default(defaultPreferences.defaultOutputFormat)
    .notNull()
});
