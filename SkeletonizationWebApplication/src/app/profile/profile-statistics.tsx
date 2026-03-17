/**
 * @file profile-statistics.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Renders profile statistics summary cards.
 * @description Composes aggregate counters into display-ready statistic cards for images, runs, and skeletonized outputs.
 */

import { StatisticCard, type StatisticCardItem } from "./profile-static-card";

type ProfileStatisticsProps = {
  imagesCount: number;
  runsCount: number;
  skeletonizedImagesCount: number;
};

/**
 * @brief Displays key profile usage statistics.
 * @description Maps numeric counters into visual cards for quick account activity overview.
 * @param imagesCount Total image count for the user.
 * @param runsCount Total skeletonization run count.
 * @param skeletonizedImagesCount Total skeletonized image count.
 * @returns A responsive grid of statistic cards.
 */
export const ProfileStatistics = ({ imagesCount, runsCount, skeletonizedImagesCount }: ProfileStatisticsProps) => {
  const statisticCardItems: StatisticCardItem[] = [
    {
      value: imagesCount,
      label: "Images",
      colorFrom: "from-cyan-50 dark:from-cyan-950/30",
      colorTo: "to-cyan-100 dark:to-cyan-900/30",
      textColor: "text-cyan-600 dark:text-cyan-400"
    },
    {
      value: runsCount,
      label: "Skeletonization Runs",
      colorFrom: "from-blue-50 dark:from-blue-950/30",
      colorTo: "to-blue-100 dark:to-blue-900/30",
      textColor: "text-blue-600 dark:text-blue-400"
    },
    {
      value: skeletonizedImagesCount,
      label: "Skeletonized Images",
      colorFrom: "from-purple-50 dark:from-purple-950/30",
      colorTo: "to-purple-100 dark:to-purple-900/30",
      textColor: "text-purple-600 dark:text-purple-400"
    }
  ];

  return (
    <div className="mt-6 grid grid-cols-1 gap-4 sm:grid-cols-3 xl:mt-5 xl:gap-3 2xl:mt-6 2xl:gap-4">
      {statisticCardItems.map((card) => (
        <StatisticCard key={card.label} item={card} />
      ))}
    </div>
  );
};
