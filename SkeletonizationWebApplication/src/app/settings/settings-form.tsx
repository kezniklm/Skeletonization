/**
 * @file settings-form.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Provides interactive user preference form controls.
 * @description Manages validated settings form state, persistence, and side effects for theme, timezone, and preference toggles.
 */

"use client";

import { zodResolver } from "@hookform/resolvers/zod";
import { Bell, Eye, Globe, Palette } from "lucide-react";
import { FormProvider, useForm } from "react-hook-form";
import { toast } from "sonner";

import { FormSelect, FormToggle } from "@/components/form";
import { type UserPreferences, userPreferencesSchema } from "@/database/zod/preferences";
import { updatePreferencesAction } from "@/server-actions/preferences";
import { useTheme } from "@/contexts/theme-context";
import { useAnimatedBackgroundPreference } from "@/contexts/animated-background-preference-context";
import { useTimezone } from "@/contexts/timezone-context";
import { defaultPreferences } from "@/database/schema";
import { detectTimezone, getTimezoneOptions } from "@/lib/timezone-utils";

import { SettingsCard } from "./settings-card";

type SettingsFormProps = {
  userId: string;
  initialPreferences: UserPreferences;
};

/**
 * @brief Renders and submits the user settings form.
 * @description Initializes preference form defaults, handles save/reset actions, and updates context providers after persistence.
 * @param userId Identifier of the user whose preferences are updated.
 * @param initialPreferences Initial preference values loaded from storage.
 * @returns A settings form with grouped preference cards.
 */
export const SettingsForm = ({ userId, initialPreferences }: SettingsFormProps) => {
  const { setTheme } = useTheme();
  const { setAnimatedBackgroundDisabled } = useAnimatedBackgroundPreference();
  const { setTimezone } = useTimezone();

  const timezoneOptions = getTimezoneOptions();

  const methods = useForm<UserPreferences>({
    resolver: zodResolver(userPreferencesSchema),
    defaultValues: {
      ...initialPreferences,
      timezone:
        initialPreferences.timezone === "UTC"
          ? detectTimezone(defaultPreferences.timezone)
          : initialPreferences.timezone
    }
  });

  const {
    handleSubmit,
    reset,
    formState: { isSubmitting }
  } = methods;

  const onSubmit = async (data: UserPreferences) => {
    try {
      await updatePreferencesAction(data, userId);
      setTheme(data.theme);
      setAnimatedBackgroundDisabled(data.animatedBackgroundDisabled);
      setTimezone(data.timezone);
      toast.success("Settings saved successfully!");
    } catch (error) {
      toast.error("Failed to save settings. Please try again.");
      console.error("Error saving preferences:", error);
    }
  };

  const handleReset = () => {
    reset({
      ...defaultPreferences,
      timezone: detectTimezone(defaultPreferences.timezone)
    });
    setTimezone(detectTimezone(defaultPreferences.timezone));
    toast.info("Settings reset to defaults.");
  };

  return (
    <FormProvider {...methods}>
      <form onSubmit={handleSubmit(onSubmit)}>
        <div className="space-y-6 xl:space-y-5 2xl:space-y-6">
          <SettingsCard
            title="Appearance"
            icon={<Palette className="h-5 w-5 xl:h-4 xl:w-4 2xl:h-5 2xl:w-5" />}
            description="Customize how the application looks"
            items={[
              {
                label: "Theme",
                description: "Choose your preferred color scheme",
                control: (
                  <FormSelect
                    name="theme"
                    options={[
                      { value: "system", label: "🖥️ System" },
                      { value: "light", label: "☀️ Light" },
                      { value: "dark", label: "🌙 Dark" }
                    ]}
                  />
                )
              },
              {
                label: "Animated Background",
                description: "Enable or disable the animated background",
                control: <FormToggle name="animatedBackgroundDisabled" aria-label="Toggle animated background" />
              }
            ]}
          />

          <SettingsCard
            title="Notifications"
            icon={<Bell className="h-5 w-5 xl:h-4 xl:w-4 2xl:h-5 2xl:w-5" />}
            description="Manage how you receive updates"
            items={[
              {
                label: "Push Notifications",
                description: "Get browser notifications for important events",
                control: <FormToggle name="pushNotifications" aria-label="Enable push notifications" />
              }
            ]}
          />

          <SettingsCard
            title="Timezone"
            icon={<Globe className="h-5 w-5 xl:h-4 xl:w-4 2xl:h-5 2xl:w-5" />}
            description="Set your preferred timezone"
            items={[
              {
                label: "Timezone",
                description: "Set your local timezone for accurate timestamps",
                control: <FormSelect name="timezone" options={timezoneOptions} />
              }
            ]}
          />

          <SettingsCard
            title="Processing Preferences"
            icon={<Eye className="h-5 w-5 xl:h-4 xl:w-4 2xl:h-5 2xl:w-5" />}
            description="Configure default processing settings"
            items={[
              {
                label: "Default Output Format",
                description: "Choose the default format for processed images",
                control: (
                  <FormSelect
                    name="defaultOutputFormat"
                    options={[
                      { value: "PNG", label: "PNG" },
                      { value: "JPEG", label: "JPEG" },
                      { value: "TIFF", label: "TIFF" },
                      { value: "BMP", label: "BMP" }
                    ]}
                  />
                )
              }
            ]}
          />
        </div>

        <div className="mt-8 flex justify-end space-x-4 xl:mt-6 xl:space-x-3 2xl:mt-8 2xl:space-x-4">
          <button
            type="button"
            onClick={handleReset}
            disabled={isSubmitting}
            className="rounded-lg border border-gray-300 bg-white px-6 py-2 text-sm font-medium text-gray-700 transition-all hover:bg-gray-50 focus:ring-2 focus:ring-cyan-500/20 focus:outline-none disabled:cursor-not-allowed disabled:opacity-50 xl:px-5 xl:py-1.5 xl:text-xs 2xl:px-6 2xl:py-2 2xl:text-sm dark:border-gray-700 dark:bg-gray-800 dark:text-gray-300 dark:hover:bg-gray-700"
          >
            Reset to Defaults
          </button>
          <button
            type="submit"
            disabled={isSubmitting}
            className="rounded-lg bg-linear-to-r from-cyan-500 to-blue-500 px-6 py-2 text-sm font-medium text-white transition-all hover:from-cyan-600 hover:to-blue-600 hover:shadow-md focus:ring-2 focus:ring-cyan-500/20 focus:outline-none disabled:cursor-not-allowed disabled:opacity-50 xl:px-5 xl:py-1.5 xl:text-xs 2xl:px-6 2xl:py-2 2xl:text-sm"
          >
            {isSubmitting ? "Saving..." : "Save Changes"}
          </button>
        </div>
      </form>
    </FormProvider>
  );
};
