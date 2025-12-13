import { eq } from "drizzle-orm/sql/expressions/conditions";

import { db } from "@/database";
import { userPreferences } from "@/database/schema/preferences";
import { type UserPreferences } from "@/database/zod/preferences";

export const getOrCreateUserPreferences = async (userId: string) => {
  const [preferences] = await db.select().from(userPreferences).where(eq(userPreferences.userId, userId));

  return preferences ?? (await createUserPreferences(userId));
};

export const getUserDefaultOutputFormatPreferences = async (userId: string) => {
  const [{ defaultOutputFormat }] = await db
    .select({ defaultOutputFormat: userPreferences.defaultOutputFormat })
    .from(userPreferences)
    .where(eq(userPreferences.userId, userId));

  return defaultOutputFormat;
};

export const getUserPushNotificationsPreferences = async (userId: string) => {
  const [{ pushNotifications }] = await db
    .select({ pushNotifications: userPreferences.pushNotifications })
    .from(userPreferences)
    .where(eq(userPreferences.userId, userId));

  return pushNotifications;
};

export const createUserPreferences = async (userId: string) => {
  const [newPreferences] = await db.insert(userPreferences).values({ userId }).returning();

  return newPreferences;
};

export const updatePreferences = async (preferencesData: UserPreferences, userId: string) => {
  const [updated] = await db
    .update(userPreferences)
    .set(preferencesData)
    .where(eq(userPreferences.userId, userId))
    .returning();

  return updated;
};
