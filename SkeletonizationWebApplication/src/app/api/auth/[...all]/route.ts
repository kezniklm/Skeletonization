/**
 * @file route.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Exposes authentication API route handlers.
 * @description Delegates Next.js GET and POST auth endpoints to Better Auth adapter.
 * @version 1.0
 * @date 2026-03-16
 */

import { toNextJsHandler } from "better-auth/next-js";

import { auth } from "@/auth";

/**
 * @brief Exports Better Auth request handlers for GET and POST.
 * @description Handles all auth provider and session endpoints under this route segment.
 */
export const { GET, POST } = toNextJsHandler(auth);
