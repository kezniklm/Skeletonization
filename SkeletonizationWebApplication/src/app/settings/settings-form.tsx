"use client";

import { zodResolver } from "@hookform/resolvers/zod";
import { Bell, Eye, Globe, Palette } from "lucide-react";
import { FormProvider, useForm } from "react-hook-form";
import { toast } from "sonner";

import { FormSelect, FormToggle } from "@/components/form";
import { type UserPreferences, userPreferencesSchema, defaultPreferences } from "@/database/zod/preferences";
import { updatePreferencesAction } from "@/server-actions/preferences";

import { SettingsCard } from "./settings-card";

type SettingsFormProps = {
  userId: string;
  initialPreferences: UserPreferences | null;
};

export const SettingsForm = ({ userId, initialPreferences }: SettingsFormProps) => {
  const methods = useForm<UserPreferences>({
    resolver: zodResolver(userPreferencesSchema),
    defaultValues: initialPreferences ?? defaultPreferences
  });

  const {
    handleSubmit,
    reset,
    formState: { isSubmitting }
  } = methods;

  const onSubmit = async (data: UserPreferences) => {
    try {
      await updatePreferencesAction(data, userId);
      toast.success("Settings saved successfully!");
    } catch (error) {
      toast.error("Failed to save settings. Please try again.");
      console.error("Error saving preferences:", error);
    }
  };

  const handleReset = () => {
    reset(defaultPreferences);
    toast.info("Settings reset to defaults");
  };

  return (
    <FormProvider {...methods}>
      <form onSubmit={handleSubmit(onSubmit)}>
        <div className="space-y-6">
          <SettingsCard
            title="Appearance"
            icon={<Palette className="h-5 w-5" />}
            description="Customize how the application looks"
            items={[
              {
                label: "Theme",
                description: "Choose your preferred color scheme",
                control: (
                  <FormSelect
                    name="theme"
                    options={[
                      { value: "system", label: "System" },
                      { value: "light", label: "Light" },
                      { value: "dark", label: "Dark" }
                    ]}
                  />
                )
              },
              {
                label: "Compact Mode",
                description: "Use a more condensed layout",
                control: <FormToggle name="compactMode" aria-label="Enable compact mode" />
              }
            ]}
          />

          <SettingsCard
            title="Notifications"
            icon={<Bell className="h-5 w-5" />}
            description="Manage how you receive updates"
            items={[
              {
                label: "Email Notifications",
                description: "Receive email updates about your processing jobs",
                control: <FormToggle name="emailNotifications" aria-label="Enable email notifications" />
              },
              {
                label: "Push Notifications",
                description: "Get browser notifications for important events",
                control: <FormToggle name="pushNotifications" aria-label="Enable push notifications" />
              }
            ]}
          />

          <SettingsCard
            title="Timezone"
            icon={<Globe className="h-5 w-5" />}
            description="Set your preferred timezone"
            items={[
              {
                label: "Timezone",
                description: "Set your local timezone for accurate timestamps",
                control: (
                  <FormSelect
                    name="timezone"
                    options={[
                      { value: "UTC", label: "UTC" },
                      { value: "Europe/Prague", label: "Europe/Prague" },
                      { value: "America/New_York", label: "America/New_York" },
                      { value: "Asia/Tokyo", label: "Asia/Tokyo" }
                    ]}
                  />
                )
              }
            ]}
          />

          <SettingsCard
            title="Processing Preferences"
            icon={<Eye className="h-5 w-5" />}
            description="Configure default processing settings"
            items={[
              {
                label: "Auto-save Results",
                description: "Automatically save processing results to your library",
                control: <FormToggle name="autoSaveResults" aria-label="Enable auto-save results" />
              },
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

        <div className="mt-8 flex justify-end space-x-4">
          <button
            type="button"
            onClick={handleReset}
            disabled={isSubmitting}
            className="rounded-lg border border-gray-300 bg-white px-6 py-2 text-sm font-medium text-gray-700 transition-all hover:bg-gray-50 focus:ring-2 focus:ring-cyan-500/20 focus:outline-none disabled:cursor-not-allowed disabled:opacity-50 dark:border-gray-700 dark:bg-gray-800 dark:text-gray-300 dark:hover:bg-gray-700"
          >
            Reset to Defaults
          </button>
          <button
            type="submit"
            disabled={isSubmitting}
            className="rounded-lg bg-gradient-to-r from-cyan-500 to-blue-500 px-6 py-2 text-sm font-medium text-white transition-all hover:from-cyan-600 hover:to-blue-600 hover:shadow-md focus:ring-2 focus:ring-cyan-500/20 focus:outline-none disabled:cursor-not-allowed disabled:opacity-50"
          >
            {isSubmitting ? "Saving..." : "Save Changes"}
          </button>
        </div>
      </form>
    </FormProvider>
  );
};
