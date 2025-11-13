type StatisticCardProps = {
  value: number;
  label: string;
  colorFrom: string;
  colorTo: string;
  textColor: string;
};

export const StatisticCard = ({ value, label, colorFrom, colorTo, textColor }: StatisticCardProps) => (
  <div
    className={`rounded-xl border border-gray-200 bg-gradient-to-br ${colorFrom} ${colorTo} p-6 text-center dark:border-gray-800`}
  >
    <div className={`text-3xl font-bold ${textColor}`}>{value}</div>
    <div className="mt-1 text-sm text-gray-600 dark:text-gray-400">{label}</div>
  </div>
);
