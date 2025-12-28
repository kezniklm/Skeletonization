import { headers } from "next/headers";
import { redirect } from "next/navigation";

import { auth } from "@/auth";
import { getRunsWithDetailsByUserId } from "@/repositories/run";
import { getUserPushNotificationsPreferences } from "@/repositories/preferences";
import { getImagesCountByUserId } from "@/repositories/image";
import { NotificationPermissionDialog } from "@/components/notification-permission-dialog";

import { LabEmptyState } from "./lab-empty-state";
import { LabHistory } from "./lab-history";

const LabPage = async () => {
  const session = await auth.api.getSession({ headers: await headers() });

  if (!session?.user) {
    redirect("/");
  }

  const [runs, pushNotificationsEnabled, imagesCount] = await Promise.all([
    getRunsWithDetailsByUserId(session.user.id),
    getUserPushNotificationsPreferences(session.user.id),
    getImagesCountByUserId(session.user.id)
  ]);

  return (
    <div className="relative z-10 mx-auto max-w-4xl px-4 sm:px-6 lg:px-0 2xl:max-w-5xl">
      <NotificationPermissionDialog notificationsEnabled={pushNotificationsEnabled} />

      <div className="mb-8">
        <h1 className="bg-linear-to-r from-cyan-600 to-blue-600 bg-clip-text text-4xl font-bold text-transparent xl:text-3xl 2xl:text-4xl dark:from-cyan-400 dark:to-blue-400">
          Processing Lab
        </h1>
        <p className="mt-2 text-gray-600 xl:text-sm 2xl:text-base dark:text-gray-400">
          Monitor your skeletonization runs and view processing history
        </p>
      </div>

      <div className="space-y-6">
        {runs.length === 0 ? <LabEmptyState hasImages={imagesCount > 0} /> : <LabHistory runs={runs} />}
      </div>
    </div>
  );
};

export default LabPage;
