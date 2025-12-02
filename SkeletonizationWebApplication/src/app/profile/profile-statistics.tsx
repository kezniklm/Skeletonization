import { StatisticCard } from "./profile-static-card";

type ProfileStatisticsProps = {
  imagesProcessed?: number;
  activeProjects?: number;
  savedAlgorithms?: number;
};

export const ProfileStatistics = ({ imagesProcessed, activeProjects, savedAlgorithms }: ProfileStatisticsProps) => {
  const statistics = [
    {
      value: imagesProcessed ?? 0,
      label: "Images Processed",
      colorFrom: "from-cyan-50 dark:from-cyan-950/30",
      colorTo: "to-cyan-100 dark:to-cyan-900/30",
      textColor: "text-cyan-600 dark:text-cyan-400"
    },
    {
      value: activeProjects ?? 0,
      label: "Active Projects",
      colorFrom: "from-blue-50 dark:from-blue-950/30",
      colorTo: "to-blue-100 dark:to-blue-900/30",
      textColor: "text-blue-600 dark:text-blue-400"
    },
    {
      value: savedAlgorithms ?? 0,
      label: "Saved Algorithms",
      colorFrom: "from-purple-50 dark:from-purple-950/30",
      colorTo: "to-purple-100 dark:to-purple-900/30",
      textColor: "text-purple-600 dark:text-purple-400"
    }
  ];

  return (
    <div className="mt-6 grid grid-cols-1 gap-4 sm:grid-cols-3 xl:mt-5 xl:gap-3 2xl:mt-6 2xl:gap-4">
      {statistics.map((stat) => (
        <StatisticCard
          key={stat.label}
          value={stat.value}
          label={stat.label}
          colorFrom={stat.colorFrom}
          colorTo={stat.colorTo}
          textColor={stat.textColor}
        />
      ))}
    </div>
  );
};
