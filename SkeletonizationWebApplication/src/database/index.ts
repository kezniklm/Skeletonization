import "dotenv/config";

import { drizzle } from "drizzle-orm/postgres-js";
import postgres from "postgres";

import { algorithm } from "./schema/algorithm";
import { account, session, user, verification } from "./schema/auth";
import { image } from "./schema/image";
import { job } from "./schema/job";
import { output } from "./schema/output";
import { userPreferences } from "./schema/preferences";
import { run } from "./schema/run";
import { runImage } from "./schema/run-image";

const client = postgres(process.env.DATABASE_URL as string);

export const db = drizzle(client, {
  schema: {
    user,
    session,
    account,
    verification,
    userPreferences,
    image,
    algorithm,
    run,
    runImage,
    output,
    job
  }
});

export type DB = typeof db;
