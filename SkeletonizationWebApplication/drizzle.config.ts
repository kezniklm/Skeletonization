/**
 * @file drizzle.config.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Defines Drizzle migration configuration.
 * @description Configures schema path, output folder, dialect, and database credentials for Drizzle CLI.
 * @version 1.0
 * @date 2026-03-16
 */

import { defineConfig } from "drizzle-kit";

export default defineConfig({
  out: "./drizzle",
  schema: "./src/database/schema",
  dialect: "postgresql",
  dbCredentials: {
    url: process.env.DATABASE_URL as string
  }
});
