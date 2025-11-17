import "dotenv/config";

import { drizzle } from "drizzle-orm/postgres-js";
import postgres from "postgres";

import { account, session, user, verification } from "./schema/auth";
import { userPreferences } from "./schema/preferences";

const client = postgres(process.env.DATABASE_URL as string);

export const db = drizzle(client, {
  schema: {
    user,
    session,
    account,
    verification,
    userPreferences
  }
});

export type DB = typeof db;
