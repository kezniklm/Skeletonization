import Image from "next/image";
import { Shield, User as UserIcon } from "lucide-react";

type ProfileHeaderProps = {
  name: string | null;
  email: string;
  image: string | null;
  emailVerified: boolean;
};

export const ProfileHeader = ({ name, email, image, emailVerified }: ProfileHeaderProps) => (
  <div className="mb-6 overflow-hidden rounded-xl border border-gray-200 bg-white shadow-sm backdrop-blur-sm xl:rounded-lg xl:shadow-sm 2xl:rounded-xl 2xl:shadow-sm dark:border-gray-800 dark:bg-gray-900/95">
    <div className="bg-linear-to-r from-cyan-500 to-blue-500 px-6 py-8 xl:px-5 xl:py-6 2xl:px-6 2xl:py-8">
      <div className="flex items-center space-x-4 xl:space-x-3 2xl:space-x-4">
        <div className="relative h-24 w-24 overflow-hidden rounded-full ring-4 ring-white xl:h-20 xl:w-20 2xl:h-24 2xl:w-24 dark:ring-gray-800">
          {image ? (
            <Image src={image} alt={name ?? "User"} fill className="object-cover" />
          ) : (
            <div className="flex h-full w-full items-center justify-center bg-linear-to-br from-cyan-100 to-blue-100 dark:from-cyan-900 dark:to-blue-900">
              <UserIcon className="h-12 w-12 text-cyan-600 xl:h-10 xl:w-10 2xl:h-12 2xl:w-12 dark:text-cyan-400" />
            </div>
          )}
        </div>
        <div className="flex-1">
          <h2 className="text-2xl font-bold text-white xl:text-xl 2xl:text-2xl">{name ?? "User"}</h2>
          <p className="mt-1 text-sm text-cyan-50 xl:text-xs 2xl:text-sm">{email}</p>
          {emailVerified && (
            <div className="mt-2 inline-flex items-center space-x-1 rounded-full bg-white/20 px-3 py-1 text-xs font-medium text-white backdrop-blur-sm xl:px-2.5 xl:py-0.5 2xl:px-3 2xl:py-1">
              <Shield className="h-3 w-3" />
              <span>Verified Account</span>
            </div>
          )}
        </div>
      </div>
    </div>
  </div>
);
