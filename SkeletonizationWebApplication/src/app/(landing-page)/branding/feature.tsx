import type { LucideIcon } from "lucide-react";

type FeatureProps = {
  icon: LucideIcon;
  title: string;
  description: string;
};

export const Feature = ({ icon: Icon, title, description }: FeatureProps) => (
  <article className="group flex items-start space-x-4 rounded-2xl bg-white/60 p-4 backdrop-blur-sm transition-all hover:bg-white/80 hover:shadow-lg xl:space-x-3 xl:rounded-xl xl:p-3 2xl:space-x-4 2xl:rounded-2xl 2xl:p-4 dark:bg-gray-800/60 dark:hover:bg-gray-800/80">
    <div
      className="flex h-12 w-12 shrink-0 items-center justify-center rounded-xl bg-linear-to-br from-cyan-600 to-blue-600 text-white shadow-lg transition-transform group-hover:scale-110 xl:h-10 xl:w-10 xl:rounded-lg 2xl:h-12 2xl:w-12 2xl:rounded-xl"
      aria-hidden="true"
    >
      <Icon className="h-6 w-6 xl:h-5 xl:w-5 2xl:h-6 2xl:w-6" />
    </div>
    <div className="flex-1">
      <h3 className="mb-1 font-semibold text-gray-900 xl:text-sm 2xl:text-base dark:text-white">{title}</h3>
      <p className="text-sm text-gray-600 xl:text-xs 2xl:text-sm dark:text-gray-400">{description}</p>
    </div>
  </article>
);
