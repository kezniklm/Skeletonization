type StatCardProps = {
  icon: string;
  value: string | number;
  label: string;
  subLabel?: string;
  gradient: string;
};

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
