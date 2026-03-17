/**
 * @file preferences.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Provides repository helpers for user preferences.
 * @description Supports retrieval, creation, and updates for per-user preferences including output format and timezone.
 * @version 1.0
 * @date 2026-03-16
 */

import { eq } from "drizzle-orm/sql/expressions/conditions";

import { db } from "@/database";
import { userPreferences } from "@/database/schema/preferences";
import { type UserPreferences } from "@/database/zod/preferences";

/** @brief Returns preferences for a user, creating defaults when missing. */
export const getOrCreateUserPreferences = async (userId: string) => {
  const [preferences] = await db.select().from(userPreferences).where(eq(userPreferences.userId, userId));

  return preferences ?? (await createUserPreferences(userId));
};

/** @brief Returns default output format preference for a user. */
export const getUserDefaultOutputFormatPreferences = async (userId: string) => {
  const [{ defaultOutputFormat }] = await db
    .select({ defaultOutputFormat: userPreferences.defaultOutputFormat })
    .from(userPreferences)
    .where(eq(userPreferences.userId, userId));

  return defaultOutputFormat;
};

/** @brief Returns push notification preference for a user. */
export const getUserPushNotificationsPreferences = async (userId: string) => {
  const [{ pushNotifications }] = await db
    .select({ pushNotifications: userPreferences.pushNotifications })
    .from(userPreferences)
    .where(eq(userPreferences.userId, userId));

  return pushNotifications;
};

/** @brief Returns timezone preference for a user. */
export const getUserTimezonePreferences = async (userId: string) => {
  const [{ timezone }] = await db
    .select({ timezone: userPreferences.timezone })
    .from(userPreferences)
    .where(eq(userPreferences.userId, userId));

  return timezone;
};

/** @brief Inserts default preferences for a user. */
export const createUserPreferences = async (userId: string) => {
  const [newPreferences] = await db.insert(userPreferences).values({ userId }).returning();

  return newPreferences;
};

/** @brief Updates preferences for a user id. */
export const updatePreferences = async (preferencesData: UserPreferences, userId: string) => {
  const [updated] = await db
    .update(userPreferences)
    .set(preferencesData)
    .where(eq(userPreferences.userId, userId))
    .returning();

  return updated;
};
