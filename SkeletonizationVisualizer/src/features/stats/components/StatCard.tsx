/**
 * @file StatCard.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Renders one metric summary tile.
 * @description Displays an icon, primary value, label, and optional secondary note with gradient background.
 * @version 1.0
 * @date 2026-03-16
 */

type StatCardProps = {
  icon: string;
  value: string | number;
  label: string;
  subLabel?: string;
  gradient: string;
};

/**
 * @brief Displays a single benchmark metric card.
 * @param icon Leading icon marker.
 * @param value Primary metric value.
 * @param label Metric title.
 * @param subLabel Optional secondary metric text.
 * @param gradient Tailwind gradient classes for card background.
 * @returns Stat card JSX.
 */
/** @brief Renders one gradient metric card for dashboard statistics. */
export const StatCard = ({ icon, value, label, subLabel, gradient }: StatCardProps) => (
  <div
    className={`transform rounded-xl bg-gradient-to-br ${gradient} p-6 text-white shadow-lg transition-all hover:-translate-y-1.5 hover:shadow-xl`}
  >
    <div className="mb-3 text-4xl">{icon}</div>
    <div>
      <div className="mb-2 text-3xl font-bold md:text-4xl">{value}</div>
      <div className="text-sm opacity-90 md:text-base">{label}</div>
      {subLabel && <div className="mt-2 text-xs opacity-75">{subLabel}</div>}
    </div>
  </div>
);
