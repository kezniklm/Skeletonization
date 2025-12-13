import { headers } from "next/headers";
import { redirect } from "next/navigation";

import { auth } from "@/auth";
import { getUserImagesByStatus } from "@/repositories/image";
import { getUserPushNotificationsPreferences } from "@/repositories/preferences";
import { NotificationPermissionDialog } from "@/components/notification-permission-dialog";

import { SkeletonizationWorkspace } from "./skeletonization-workspace";
import { SkeletonizationEmptyState } from "./skeletonization-empty-state";

const SkeletonizationPage = async () => {
  const session = await auth.api.getSession({ headers: await headers() });

  if (!session?.user) {
    redirect("/");
  }

  const [uploadedImages, validatedImages, pushNotificationsEnabled] = await Promise.all([
    getUserImagesByStatus(session.user.id, "uploaded"),
    getUserImagesByStatus(session.user.id, "validated"),
    getUserPushNotificationsPreferences(session.user.id)
  ]);

  const availableImages = [...uploadedImages, ...validatedImages];

  return (
    <div className="relative z-10 mx-auto max-w-4xl px-4 sm:px-6 lg:px-0 2xl:max-w-5xl">
      <NotificationPermissionDialog notificationsEnabled={pushNotificationsEnabled} />

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
        <SkeletonizationWorkspace images={availableImages} />
      )}
    </div>
  );
};

export default SkeletonizationPage;
