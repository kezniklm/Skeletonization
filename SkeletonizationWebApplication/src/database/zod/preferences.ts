/**
 * @file preferences.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Defines Zod schemas and types for user preferences.
 * @description Builds validated preference schema with timezone refinement and exports inferred preference-related types.
 * @version 1.0
 * @date 2026-03-16
 */

import { createInsertSchema } from "drizzle-zod";
import { type z } from "zod";

import { isValidTimezone } from "@/lib/timezone-utils";

import { userPreferences } from "../schema/preferences";

/** @brief Required schema for mutable user preference values. */
export const userPreferencesSchema = createInsertSchema(userPreferences)
  .omit({ userId: true })
  .extend({
    timezone: createInsertSchema(userPreferences).shape.timezone.refine((tz) => isValidTimezone(tz ?? ""), {
      message: "Invalid timezone"
    })
  })
  .required();

/** @brief User preferences type inferred from schema. */
export type UserPreferences = z.infer<typeof userPreferencesSchema>;

/** @brief Output file format type inferred from preference schema. */
export type FileOutputFormat = z.infer<typeof userPreferencesSchema.shape.defaultOutputFormat>;

/** @brief Lowercase image format helper type. */
export type ImageFormat = Lowercase<FileOutputFormat>;

/** @brief Theme preference type inferred from schema. */
export type Theme = z.infer<typeof userPreferencesSchema.shape.theme>;
