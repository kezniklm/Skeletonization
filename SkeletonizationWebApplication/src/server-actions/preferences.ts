"use server";

import { headers } from "next/headers";

import { auth } from "@/auth";
import { userPreferencesSchema, type UserPreferences } from "@/database/zod/preferences";
import { updatePreferences } from "@/repositories/preferences";

export const updatePreferencesAction = async (preferencesData: UserPreferences, userId: string) => {
  const userPreferences = userPreferencesSchema.parse(preferencesData);

  const session = await auth.api.getSession({ headers: await headers() });

  if (!session?.user?.id) {
    throw new Error("You must be logged in to update user preferences!");
  }

  return updatePreferences(userPreferences, userId);
};
