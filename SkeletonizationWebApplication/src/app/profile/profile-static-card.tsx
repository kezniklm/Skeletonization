export type StatisticCardItem = {
  value: number;
  label: string;
  colorFrom: string;
  colorTo: string;
  textColor: string;
};

type StatisticCardProps = {
  item: StatisticCardItem;
};

export const StatisticCard = ({ item }: StatisticCardProps) => (
  <div
    className={`rounded-xl border border-gray-200 bg-linear-to-br ${item.colorFrom} ${item.colorTo} p-6 text-center xl:rounded-lg xl:p-5 2xl:rounded-xl 2xl:p-6 dark:border-gray-800`}
  >
    <div className={`text-3xl font-bold ${item.textColor} xl:text-2xl 2xl:text-3xl`}>{item.value}</div>
    <div className="mt-1 text-sm text-gray-600 xl:text-xs 2xl:text-sm dark:text-gray-400">{item.label}</div>
  </div>
);
