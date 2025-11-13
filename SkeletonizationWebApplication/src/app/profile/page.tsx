import { headers } from "next/headers";
import { redirect } from "next/navigation";
import { Calendar } from "lucide-react";

import { auth } from "@/auth";

import { ProfileHeader } from "./profile-header";
import { ProfileSection } from "./profile-section";
import { ProfileStatistics } from "./profile-statistics";

const ProfilePage = async () => {
  const session = await auth.api.getSession({ headers: await headers() });

  if (!session?.user) {
    redirect("/");
  }

  const { user } = session;

  const accountSection = {
    title: "Account Information",
    icon: <Calendar className="h-5 w-5" />,
    fields: [
      {
        label: "Member Since",
        value: user.createdAt
          ? new Date(user.createdAt).toLocaleDateString("en-US", {
              year: "numeric",
              month: "long",
              day: "numeric"
            })
          : "N/A",
        icon: <Calendar className="h-4 w-4 text-gray-400" />
      },
      {
        label: "Last Updated",
        value: user.updatedAt
          ? new Date(user.updatedAt).toLocaleDateString("en-US", {
              year: "numeric",
              month: "long",
              day: "numeric"
            })
          : "N/A",
        icon: <Calendar className="h-4 w-4 text-gray-400" />
      }
    ]
  };

  //TODO Statistics
  return (
    <div className="mx-auto max-w-4xl">
      <div className="mb-8">
        <h1 className="bg-gradient-to-r from-cyan-600 to-blue-600 bg-clip-text text-4xl font-bold text-transparent dark:from-cyan-400 dark:to-blue-400">
          Profile
        </h1>
        <p className="mt-2 text-gray-600 dark:text-gray-400">View and manage your profile information</p>
      </div>

      <ProfileHeader
        name={user.name}
        email={user.email}
        image={user.image ?? null}
        emailVerified={user.emailVerified}
      />

      <div className="space-y-6">
        <ProfileSection title={accountSection.title} icon={accountSection.icon} fields={accountSection.fields} />
      </div>

      <ProfileStatistics />
    </div>
  );
};

export default ProfilePage;
