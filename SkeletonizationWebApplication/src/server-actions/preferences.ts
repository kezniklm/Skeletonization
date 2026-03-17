/**
 * @file preferences.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Server action for updating user preferences.
 * @description Validates preference payload and persists updates for authenticated users.
 * @version 1.0
 * @date 2026-03-16
 */

"use server";

import { userPreferencesSchema, type UserPreferences } from "@/database/zod/preferences";
import { updatePreferences } from "@/repositories/preferences";

import { requireUser } from "./common";

/** @brief Validates and updates preferences for a user id. */
export const updatePreferencesAction = async (preferencesData: UserPreferences, userId: string) => {
  await requireUser("update user preferences");

  const userPreferences = userPreferencesSchema.parse(preferencesData);

  return updatePreferences(userPreferences, userId);
};
