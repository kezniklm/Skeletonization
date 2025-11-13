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
      colorFrom: "from-cyan-50",
      colorTo: "to-cyan-100",
      textColor: "text-cyan-600 dark:text-cyan-400",
      darkFrom: "dark:from-cyan-950/30",
      darkTo: "dark:to-cyan-900/30"
    },
    {
      value: activeProjects ?? 0,
      label: "Active Projects",
      colorFrom: "from-blue-50",
      colorTo: "to-blue-100",
      textColor: "text-blue-600 dark:text-blue-400",
      darkFrom: "dark:from-blue-950/30",
      darkTo: "dark:to-blue-900/30"
    },
    {
      value: savedAlgorithms ?? 0,
      label: "Saved Algorithms",
      colorFrom: "from-purple-50",
      colorTo: "to-purple-100",
      textColor: "text-purple-600 dark:text-purple-400",
      darkFrom: "dark:from-purple-950/30",
      darkTo: "dark:to-purple-900/30"
    }
  ];

  return (
    <div className="mt-6 grid grid-cols-1 gap-4 sm:grid-cols-3">
      {statistics.map((stat) => (
        <StatisticCard
          key={stat.label}
          value={stat.value}
          label={stat.label}
          colorFrom={`${stat.colorFrom} ${stat.darkFrom}`}
          colorTo={`${stat.colorTo} ${stat.darkTo}`}
          textColor={stat.textColor}
        />
      ))}
    </div>
  );
};
