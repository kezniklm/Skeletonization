import { headers } from "next/headers";
import { redirect } from "next/navigation";

import { auth } from "@/auth";
import { getUserImagesByStatuses } from "@/repositories/image";
import { NotificationPermissionDialog } from "@/components/notification-permission-dialog";
import { getOrCreateUserPreferences } from "@/repositories/preferences";

import { SkeletonizationWorkspace } from "./skeletonization-workspace";
import { SkeletonizationEmptyState } from "./skeletonization-empty-state";

const SkeletonizationPage = async () => {
  const session = await auth.api.getSession({ headers: await headers() });

  if (!session?.user) {
    redirect("/");
  }

  const [availableImages, userPreferences] = await Promise.all([
    getUserImagesByStatuses(session.user.id, ["uploaded", "derived"]),
    getOrCreateUserPreferences(session.user.id)
  ]);

  return (
    <div className="relative z-10 mx-auto max-w-4xl px-4 sm:px-6 lg:px-0 2xl:max-w-5xl">
      <NotificationPermissionDialog notificationsEnabled={userPreferences.pushNotifications} />

      <div className="mb-8 lg:mb-4 2xl:mb-8">
        <h1 className="bg-linear-to-r from-cyan-600 to-blue-600 bg-clip-text text-4xl font-bold text-transparent xl:text-3xl 2xl:text-4xl dark:from-cyan-400 dark:to-blue-400">
          Skeletonization
        </h1>
        <p className="mt-2 text-gray-600 xl:text-sm 2xl:text-base dark:text-gray-400">
          Select images and an algorithm to start batch skeletonization processing
        </p>
      </div>

      {availableImages.length === 0 ? (
        <SkeletonizationEmptyState />
      ) : (
        <SkeletonizationWorkspace images={availableImages} defaultOutputFormat={userPreferences.defaultOutputFormat} />
      )}
    </div>
  );
};

export default SkeletonizationPage;
