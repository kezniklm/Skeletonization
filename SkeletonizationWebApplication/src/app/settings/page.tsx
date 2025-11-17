import { headers } from "next/headers";
import { redirect } from "next/navigation";

import { auth } from "@/auth";
import { getUserPreferences } from "@/repositories/preferences";

import { SettingsForm } from "./settings-form";

const SettingsPage = async () => {
  const session = await auth.api.getSession({ headers: await headers() });

  if (!session?.user) {
    redirect("/");
  }

  const userPreferences = (await getUserPreferences(session.user.id)) ?? null;

  return (
    <div className="mx-auto max-w-4xl">
      <div className="mb-8">
        <h1 className="bg-gradient-to-r from-cyan-600 to-blue-600 bg-clip-text text-4xl font-bold text-transparent dark:from-cyan-400 dark:to-blue-400">
          Settings
        </h1>
        <p className="mt-2 text-gray-600 dark:text-gray-400">Manage your account settings and preferences</p>
      </div>

      <SettingsForm userId={session.user.id} initialPreferences={userPreferences} />
    </div>
  );
};

export default SettingsPage;
