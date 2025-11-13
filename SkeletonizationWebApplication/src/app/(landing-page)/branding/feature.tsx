import type { LucideIcon } from "lucide-react";

type FeatureProps = {
  icon: LucideIcon;
  title: string;
  description: string;
};

export const Feature = ({ icon: Icon, title, description }: FeatureProps) => (
  <article className="group flex items-start space-x-4 rounded-2xl bg-white/60 p-4 backdrop-blur-sm transition-all hover:bg-white/80 hover:shadow-lg dark:bg-gray-800/60 dark:hover:bg-gray-800/80">
    <div
      className="flex h-12 w-12 flex-shrink-0 items-center justify-center rounded-xl bg-gradient-to-br from-cyan-600 to-blue-600 text-white shadow-lg transition-transform group-hover:scale-110"
      aria-hidden="true"
    >
      <Icon className="h-6 w-6" />
    </div>
    <div className="flex-1">
      <h3 className="mb-1 font-semibold text-gray-900 dark:text-white">{title}</h3>
      <p className="text-sm text-gray-600 dark:text-gray-400">{description}</p>
    </div>
  </article>
);
