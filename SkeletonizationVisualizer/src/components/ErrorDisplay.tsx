import { useTheme } from "../contexts/ThemeContext";

type ErrorDisplayProps = {
  title?: string;
  message: string;
  retry?: () => void;
};

export const ErrorDisplay = ({ title = "Error", message, retry }: ErrorDisplayProps) => {
  const { getThemeClasses } = useTheme();
  const themeClasses = getThemeClasses();

  return (
    <div
      className={`${themeClasses.bgSecondary} rounded-xl border ${themeClasses.border} mx-auto max-w-md p-8 text-center`}
    >
      <div className="mb-4 text-6xl">⚠️</div>
      <h3 className={`text-xl font-bold ${themeClasses.text} mb-2`}>{title}</h3>
      <p className={`${themeClasses.textMuted} mb-6`}>{message}</p>
      {retry && (
        <button
          onClick={retry}
          className="rounded-lg bg-[#667eea] px-6 py-2 font-medium text-white shadow-sm transition-all hover:-translate-y-0.5 hover:shadow-[0_4px_12px_rgba(102,126,234,0.4)]"
        >
          🔄 Try Again
        </button>
      )}
    </div>
  );
};
