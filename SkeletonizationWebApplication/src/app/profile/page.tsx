import { headers } from "next/headers";
import { redirect } from "next/navigation";

import { auth } from "@/auth";
import { getImagesCountByUserId, getImagesCountByUserIdAndStatus, getRunCountByUserId } from "@/repositories";

import { ProfileHeader } from "./profile-header";
import { ProfileSection } from "./profile-section";
import { ProfileStatistics } from "./profile-statistics";
import { getProfileSections } from "./sections";

const ProfilePage = async () => {
  const session = await auth.api.getSession({ headers: await headers() });

  if (!session?.user) {
    redirect("/");
  }

  const user = session.user;

  const [userImagesCount, userRunsCount, userSkeletonizedImagesCount] = await Promise.all([
    getImagesCountByUserId(user.id),
    getRunCountByUserId(user.id),
    getImagesCountByUserIdAndStatus(user.id, "skeletonized")
  ]);

  return (
    <div className="mx-auto max-w-4xl px-4 sm:px-6 lg:px-0 2xl:max-w-5xl">
      <div className="mb-8">
        <h1 className="bg-linear-to-r from-cyan-600 to-blue-600 bg-clip-text text-4xl font-bold text-transparent xl:text-3xl 2xl:text-4xl dark:from-cyan-400 dark:to-blue-400">
          Profile
        </h1>
        <p className="mt-2 text-gray-600 xl:text-sm 2xl:text-base dark:text-gray-400">
          View and manage your profile information
        </p>
      </div>

      <ProfileHeader
        name={user.name}
        email={user.email}
        image={user.image ?? null}
        emailVerified={user.emailVerified}
      />

      <div className="space-y-6 xl:space-y-5 2xl:space-y-6">
        {getProfileSections(user).map((section) => (
          <ProfileSection key={section.title} title={section.title} icon={section.icon} fields={section.fields} />
        ))}
      </div>

      <ProfileStatistics
        imagesCount={userImagesCount}
        runsCount={userRunsCount}
        skeletonizedImagesCount={userSkeletonizedImagesCount}
      />
    </div>
  );
};

export default ProfilePage;
