import { drizzle } from "drizzle-orm/postgres-js";
import postgres from "postgres";

import { account, session, user, verification } from "./schema/auth";
import { image } from "./schema/image";
import { job } from "./schema/job";
import { jobStats } from "./schema/job-stats";
import { userPreferences } from "./schema/preferences";
import { run } from "./schema/run";

const client = postgres(process.env.DATABASE_URL as string);

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

export type DB = typeof db;
