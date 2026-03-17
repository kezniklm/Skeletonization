/**
 * @file page.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Renders the authenticated settings page.
 * @description Loads user preference data and renders settings controls for account customization.
 */

import { headers } from "next/headers";
import { redirect } from "next/navigation";

import { auth } from "@/auth";
import { getOrCreateUserPreferences } from "@/repositories/preferences";

import { SettingsForm } from "./settings-form";

/**
 * @brief Loads preference data and renders settings form.
 * @description Ensures an authenticated session, fetches user preferences, and returns settings page content.
 * @returns Server-rendered settings page component.
 */
const SettingsPage = async () => {
  const session = await auth.api.getSession({ headers: await headers() });

  if (!session?.user) {
    redirect("/");
  }

  const userPreferences = await getOrCreateUserPreferences(session.user.id);

  return (
    <div className="mx-auto max-w-4xl px-4 sm:px-6 lg:px-0 2xl:max-w-5xl">
      <div className="mb-8">
        <h1 className="bg-linear-to-r from-cyan-600 to-blue-600 bg-clip-text pb-0.5 text-4xl font-bold text-transparent xl:text-3xl 2xl:text-4xl dark:from-cyan-400 dark:to-blue-400">
          Settings
        </h1>
        <p className="mt-2 text-gray-600 xl:text-sm 2xl:text-base dark:text-gray-400">
          Manage your account settings and preferences
        </p>
      </div>

      <SettingsForm userId={session.user.id} initialPreferences={userPreferences} />
    </div>
  );
};

/**
 * @brief Exposes the settings route page component.
 */
export default SettingsPage;
