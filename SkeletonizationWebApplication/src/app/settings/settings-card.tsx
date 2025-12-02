import type { ReactNode } from "react";

type SettingsItem = {
  label: string;
  description?: string;
  control: ReactNode;
};

type SettingsCardProps = {
  title: string;
  icon: ReactNode;
  description?: string;
  items: SettingsItem[];
};

export const SettingsCard = ({ title, icon, description, items }: SettingsCardProps) => (
  <div className="overflow-hidden rounded-xl border border-gray-200 bg-white shadow-sm backdrop-blur-sm transition-shadow hover:shadow-md xl:rounded-lg xl:shadow-sm 2xl:rounded-xl 2xl:shadow-md dark:border-gray-800 dark:bg-gray-900/95">
    <div className="border-b border-gray-200 bg-linear-to-r from-cyan-50/50 to-blue-50/50 px-6 py-4 xl:px-5 xl:py-3 2xl:px-6 2xl:py-4 dark:border-gray-800 dark:from-cyan-950/20 dark:to-blue-950/20">
      <div className="flex items-center space-x-3 xl:space-x-2.5 2xl:space-x-3">
        <div className="flex h-10 w-10 items-center justify-center rounded-lg bg-linear-to-r from-cyan-500 to-blue-500 text-white xl:h-9 xl:w-9 2xl:h-10 2xl:w-10">
          {icon}
        </div>
        <div>
          <h2 className="text-lg font-semibold text-gray-900 xl:text-base 2xl:text-lg dark:text-white">{title}</h2>
          {description && (
            <p className="text-sm text-gray-600 xl:text-xs 2xl:text-sm dark:text-gray-400">{description}</p>
          )}
        </div>
      </div>
    </div>
    <div className="divide-y divide-gray-200 dark:divide-gray-800">
      {items.map((item) => (
        <div key={item.label} className="px-6 py-4 xl:px-5 xl:py-3 2xl:px-6 2xl:py-4">
          <div className="flex items-center justify-between">
            <div className="flex-1">
              <h3 className="font-medium text-gray-900 xl:text-sm 2xl:text-base dark:text-white">{item.label}</h3>
              {item.description && (
                <p className="mt-1 text-sm text-gray-600 xl:text-xs 2xl:text-sm dark:text-gray-400">
                  {item.description}
                </p>
              )}
            </div>
            <div className="ml-4 xl:ml-3 2xl:ml-4">{item.control}</div>
          </div>
        </div>
      ))}
    </div>
  </div>
);
