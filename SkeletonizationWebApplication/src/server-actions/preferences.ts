"use server";

import { userPreferencesSchema, type UserPreferences } from "@/database/zod/preferences";
import { updatePreferences } from "@/repositories/preferences";

import { requireUser } from "./common";

export const updatePreferencesAction = async (preferencesData: UserPreferences, userId: string) => {
  await requireUser("update user preferences");

  const userPreferences = userPreferencesSchema.parse(preferencesData);

  return updatePreferences(userPreferences, userId);
};
