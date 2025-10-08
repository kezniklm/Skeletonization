import { useTheme } from "../contexts/ThemeContext";

type LoadingSpinnerProps = {
  message?: string;
  size?: "sm" | "md" | "lg";
};

export const LoadingSpinner = ({ message = "Loading...", size = "md" }: LoadingSpinnerProps) => {
  const { getThemeClasses } = useTheme();
  const themeClasses = getThemeClasses();

  const sizeClasses = {
    sm: "w-6 h-6",
    md: "w-12 h-12",
    lg: "w-16 h-16"
  };

  const textSizeClasses = {
    sm: "text-sm",
    md: "text-base",
    lg: "text-lg"
  };

  return (
    <div className="flex flex-col items-center justify-center p-8">
      <div
        className={`${sizeClasses[size]} animate-spin rounded-full border-4 border-[#667eea]/20 border-t-[#667eea]`}
      />
      <p className={`mt-4 ${textSizeClasses[size]} font-medium ${themeClasses.textMuted}`}>{message}</p>
    </div>
  );
};
