/**
 * @file index.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Re-exports application context providers and hooks.
 * @description Centralizes context module exports for simplified imports across the app.
 * @version 1.0
 * @date 2026-03-16
 */

export {
  AnimatedBackgroundPreferenceProvider,
  useAnimatedBackgroundPreference
} from "./animated-background-preference-context";
export { ThemeProvider, useTheme } from "./theme-context";
export { TimezoneProvider, useTimezone } from "./timezone-context";
