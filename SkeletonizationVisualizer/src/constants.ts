/**
 * @file constants.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Defines shared frontend configuration constants.
 * @description Stores timing and UI defaults used across benchmark visualizer hooks and components.
 * @version 1.0
 * @date 2026-03-16
 */

/**
 * @brief Provides application-level configuration values.
 * @description Contains refresh and comparison defaults used by data and modal hooks.
 */
export const APP_CONFIG = {
  DATA_REFRESH_INTERVAL: 2000, // ms
  SLIDESHOW_INTERVAL: 3000, // ms
  DEFAULT_SLIDER_POSITION: 50 // %
} as const;
