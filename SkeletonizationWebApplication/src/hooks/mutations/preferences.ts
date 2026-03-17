/**
 * @file preferences.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Exposes mutation hook for updating user preferences.
 * @description Wraps preference update server action in a react-query mutation for client-side usage.
 * @version 1.0
 * @date 2026-03-16
 */

import { useMutation } from "@tanstack/react-query";

import { type UserPreferences } from "@/database/zod/preferences";
import { updatePreferencesAction } from "@/server-actions/preferences";

/**
 * @brief Creates a mutation for persisting user preferences.
 * @description Returns a react-query mutation that calls the preferences server action.
 * @returns Preference update mutation object.
 */
export const useUpdatePreferencesMutation = () =>
  useMutation({
    mutationFn: async ({ preferences, userId }: { preferences: UserPreferences; userId: string }) =>
      await updatePreferencesAction(preferences, userId)
  });
