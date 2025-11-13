import Image from "next/image";
import { Shield, User as UserIcon } from "lucide-react";

type ProfileHeaderProps = {
  name: string | null;
  email: string;
  image: string | null;
  emailVerified: boolean;
};

export const ProfileHeader = ({ name, email, image, emailVerified }: ProfileHeaderProps) => (
  <div className="mb-6 overflow-hidden rounded-xl border border-gray-200 bg-white shadow-sm backdrop-blur-sm dark:border-gray-800 dark:bg-gray-900/95">
    <div className="bg-gradient-to-r from-cyan-500 to-blue-500 px-6 py-8">
      <div className="flex items-center space-x-4">
        <div className="relative h-24 w-24 overflow-hidden rounded-full ring-4 ring-white dark:ring-gray-800">
          {image ? (
            <Image src={image} alt={name ?? "User"} fill className="object-cover" />
          ) : (
            <div className="flex h-full w-full items-center justify-center bg-gradient-to-br from-cyan-100 to-blue-100 dark:from-cyan-900 dark:to-blue-900">
              <UserIcon className="h-12 w-12 text-cyan-600 dark:text-cyan-400" />
            </div>
          )}
        </div>
        <div className="flex-1">
          <h2 className="text-2xl font-bold text-white">{name}</h2>
          <p className="mt-1 text-cyan-50">{email}</p>
          {emailVerified && (
            <div className="mt-2 inline-flex items-center space-x-1 rounded-full bg-white/20 px-3 py-1 text-xs font-medium text-white backdrop-blur-sm">
              <Shield className="h-3 w-3" />
              <span>Verified Account</span>
            </div>
          )}
        </div>
      </div>
    </div>
  </div>
);
