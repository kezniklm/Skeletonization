/**
 * @file index.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Initializes database client and Drizzle schema mapping.
 * @description Creates shared Postgres client and exports typed Drizzle database instance with all table schemas.
 * @version 1.0
 * @date 2026-03-16
 */

import { drizzle } from "drizzle-orm/postgres-js";
import postgres from "postgres";

import { account, session, user, verification } from "./schema/auth";
import { image } from "./schema/image";
import { job } from "./schema/job";
import { jobStats } from "./schema/job-stats";
import { userPreferences } from "./schema/preferences";
import { run } from "./schema/run";

const client = postgres(process.env.DATABASE_URL as string);

/** @brief Shared Drizzle database instance configured with application schema. */
export const db = drizzle(client, {
  schema: {
    user,
    session,
    account,
    verification,
    userPreferences,
    image,
    run,
    job,
    jobStats
  }
});

/** @brief Type alias for configured Drizzle database instance. */
export type DB = typeof db;
