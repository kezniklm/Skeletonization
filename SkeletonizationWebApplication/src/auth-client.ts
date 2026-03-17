/**
 * @file auth-client.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Initializes Better Auth client for frontend usage.
 * @description Creates client instance configured with optional base URL for auth API routes.
 * @version 1.0
 * @date 2026-03-16
 */

import { createAuthClient } from "better-auth/react";

const base = process.env.NEXT_PUBLIC_BASE_URL;
/** @brief Shared frontend authentication client instance. */
export const authClient = createAuthClient({
  baseURL: base ? `${base}/api/auth` : undefined
});
