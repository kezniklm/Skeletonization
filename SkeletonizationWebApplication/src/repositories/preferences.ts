import { eq } from "drizzle-orm/sql/expressions/conditions";

import { db } from "@/database";
import { userPreferences } from "@/database/schema/preferences";
import { type UserPreferences } from "@/database/zod/preferences";

export const getUserPreferences = async (userId: string) => {
  const [preferences] = await db.select().from(userPreferences).where(eq(userPreferences.userId, userId));

  return preferences ?? null;
};

export const createUserPreferences = async (userId: string) => {
  const [newPreferences] = await db.insert(userPreferences).values({ userId }).returning();

  return newPreferences;
};

export const upsertUserPreferences = async (preferencesData: Partial<UserPreferences>, userId: string) => {
  const existing = await getUserPreferences(userId);

  if (!existing) {
    return createUserPreferences(userId);
  }

  const [updated] = await db
    .update(userPreferences)
    .set(preferencesData)
    .where(eq(userPreferences.userId, userId))
    .returning();

  return updated;
};

export const updatePreferences = async (preferencesData: UserPreferences, userId: string) =>
  upsertUserPreferences(preferencesData, userId);
