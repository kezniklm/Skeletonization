/**
 * @file auth.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Configures Better Auth server instance.
 * @description Wires Better Auth to Drizzle adapter and social providers for server-side authentication.
 * @version 1.0
 * @date 2026-03-16
 */

import { betterAuth } from "better-auth";
import { drizzleAdapter } from "better-auth/adapters/drizzle";
import { nextCookies } from "better-auth/next-js";

import { db } from "./database";

/** @brief Shared server authentication configuration instance. */
export const auth = betterAuth({
  database: drizzleAdapter(db, {
    provider: "pg"
  }),
  socialProviders: {
    google: {
      clientId: process.env.GOOGLE_CLIENT_ID as string,
      clientSecret: process.env.GOOGLE_CLIENT_SECRET as string
    },
    github: {
      clientId: process.env.GITHUB_CLIENT_ID as string,
      clientSecret: process.env.GITHUB_CLIENT_SECRET as string
    },
    discord: {
      clientId: process.env.DISCORD_CLIENT_ID as string,
      clientSecret: process.env.DISCORD_CLIENT_SECRET as string
    }
  },
  plugins: [nextCookies()]
});
