import { useMutation } from "@tanstack/react-query";

import { type UserPreferences } from "@/database/zod/preferences";
import { updatePreferencesAction } from "@/server-actions/preferences";

export const useUpdatePreferencesMutation = () =>
  useMutation({
    mutationFn: async ({ preferences, userId }: { preferences: UserPreferences; userId: string }) =>
      await updatePreferencesAction(preferences, userId)
  });
