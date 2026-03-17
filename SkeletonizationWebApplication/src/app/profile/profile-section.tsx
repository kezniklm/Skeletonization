/**
 * @file profile-section.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Renders a structured profile information section.
 * @description Displays labeled profile fields with icons and optional status badges under a titled section header.
 */

import type { ReactElement } from "react";
import { Shield } from "lucide-react";

type ProfileField = {
  label: string;
  value: string | null;
  icon: ReactElement;
  badge?: boolean;
};

type ProfileSectionProps = {
  title: string;
  icon: ReactElement;
  fields: ProfileField[];
};

/**
 * @brief Displays a profile data section card.
 * @description Renders a section title with icon and iterates through formatted profile fields.
 * @param title Section title text.
 * @param icon Section icon element.
 * @param fields Field entries to display in the section.
 * @returns A profile information section card.
 */
export const ProfileSection = ({ title, icon, fields }: ProfileSectionProps) => (
  <div className="overflow-hidden rounded-xl border border-gray-200 bg-white shadow-sm backdrop-blur-sm transition-shadow hover:shadow-md xl:rounded-lg xl:shadow-sm 2xl:rounded-xl 2xl:shadow-md dark:border-gray-800 dark:bg-gray-900/95">
    <div className="border-b border-gray-200 bg-linear-to-r from-cyan-50/50 to-blue-50/50 px-6 py-4 xl:px-5 xl:py-3 2xl:px-6 2xl:py-4 dark:border-gray-800 dark:from-cyan-950/20 dark:to-blue-950/20">
      <div className="flex items-center space-x-3 xl:space-x-2.5 2xl:space-x-3">
        <div className="flex h-10 w-10 items-center justify-center rounded-lg bg-linear-to-r from-cyan-500 to-blue-500 text-white xl:h-9 xl:w-9 2xl:h-10 2xl:w-10">
          {icon}
        </div>
        <h3 className="text-lg font-semibold text-gray-900 xl:text-base 2xl:text-lg dark:text-white">{title}</h3>
      </div>
    </div>
    <div className="divide-y divide-gray-200 dark:divide-gray-800">
      {fields.map((field) => (
        <div key={field.label} className="px-6 py-4 xl:px-5 xl:py-3 2xl:px-6 2xl:py-4">
          <div className="flex items-start justify-between">
            <div className="flex items-start space-x-3 xl:space-x-2.5 2xl:space-x-3">
              <div className="mt-0.5">{field.icon}</div>
              <div>
                <p className="text-sm font-medium text-gray-600 xl:text-xs 2xl:text-sm dark:text-gray-400">
                  {field.label}
                </p>
                <p className="mt-1 text-base text-gray-900 xl:text-sm 2xl:text-base dark:text-white">
                  {field.value}
                  {field.badge && (
                    <span className="ml-2 inline-flex items-center rounded-full bg-green-100 px-2.5 py-0.5 text-xs font-medium text-green-800 dark:bg-green-900/30 dark:text-green-400">
                      <Shield className="mr-1 h-3 w-3" />
                      Verified
                    </span>
                  )}
                </p>
              </div>
            </div>
          </div>
        </div>
      ))}
    </div>
  </div>
);
