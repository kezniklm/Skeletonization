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

export const ProfileSection = ({ title, icon, fields }: ProfileSectionProps) => (
  <div className="overflow-hidden rounded-xl border border-gray-200 bg-white shadow-sm backdrop-blur-sm transition-shadow hover:shadow-md dark:border-gray-800 dark:bg-gray-900/95">
    <div className="border-b border-gray-200 bg-gradient-to-r from-cyan-50/50 to-blue-50/50 px-6 py-4 dark:border-gray-800 dark:from-cyan-950/20 dark:to-blue-950/20">
      <div className="flex items-center space-x-3">
        <div className="flex h-10 w-10 items-center justify-center rounded-lg bg-gradient-to-r from-cyan-500 to-blue-500 text-white">
          {icon}
        </div>
        <h3 className="text-lg font-semibold text-gray-900 dark:text-white">{title}</h3>
      </div>
    </div>
    <div className="divide-y divide-gray-200 dark:divide-gray-800">
      {fields.map((field) => (
        <div key={field.label} className="px-6 py-4">
          <div className="flex items-start justify-between">
            <div className="flex items-start space-x-3">
              <div className="mt-0.5">{field.icon}</div>
              <div>
                <p className="text-sm font-medium text-gray-600 dark:text-gray-400">{field.label}</p>
                <p className="mt-1 text-base text-gray-900 dark:text-white">
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
