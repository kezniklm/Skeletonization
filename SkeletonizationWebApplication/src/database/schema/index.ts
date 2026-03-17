/**
 * @file index.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Re-exports database table schema modules.
 * @description Provides a unified schema entry point for auth, image, job, preferences, and run tables.
 * @version 1.0
 * @date 2026-03-16
 */

export * from "./auth";
export * from "./image";
export * from "./job";
export * from "./job-stats";
export * from "./preferences";
export * from "./run";
