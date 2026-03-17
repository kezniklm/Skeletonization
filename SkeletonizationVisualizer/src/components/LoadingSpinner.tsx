/**
 * @file LoadingSpinner.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Provides themed loading feedback component.
 * @description Displays spinner animation with configurable size and optional status text.
 * @version 1.0
 * @date 2026-03-16
 */

import { useTheme } from "../contexts/ThemeContext";

type LoadingSpinnerProps = {
  message?: string;
  size?: "sm" | "md" | "lg";
};

/**
 * @brief Renders a loading spinner with optional message.
 * @param message Optional loading text.
 * @param size Spinner size variant.
 * @returns Loading indicator JSX.
 */
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
