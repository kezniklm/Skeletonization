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
  <div className="overflow-hidden rounded-xl border border-gray-200 bg-white shadow-sm backdrop-blur-sm transition-shadow hover:shadow-md dark:border-gray-800 dark:bg-gray-900/95">
    <div className="border-b border-gray-200 bg-gradient-to-r from-cyan-50/50 to-blue-50/50 px-6 py-4 dark:border-gray-800 dark:from-cyan-950/20 dark:to-blue-950/20">
      <div className="flex items-center space-x-3">
        <div className="flex h-10 w-10 items-center justify-center rounded-lg bg-gradient-to-r from-cyan-500 to-blue-500 text-white">
          {icon}
        </div>
        <div>
          <h2 className="text-lg font-semibold text-gray-900 dark:text-white">{title}</h2>
          <p className="text-sm text-gray-600 dark:text-gray-400">{description}</p>
        </div>
      </div>
    </div>
    <div className="divide-y divide-gray-200 dark:divide-gray-800">
      {items.map((item) => (
        <div key={item.label} className="px-6 py-4">
          <div className="flex items-center justify-between">
            <div className="flex-1">
              <h3 className="font-medium text-gray-900 dark:text-white">{item.label}</h3>
              <p className="mt-1 text-sm text-gray-600 dark:text-gray-400">{item.description}</p>
            </div>
            <div className="ml-4">{item.control}</div>
          </div>
        </div>
      ))}
    </div>
  </div>
);
