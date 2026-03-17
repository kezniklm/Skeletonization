/**
 * @file common.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Shared server-action authentication utilities.
 * @description Provides helper for enforcing authenticated user context in server actions.
 * @version 1.0
 * @date 2026-03-16
 */

import { headers } from "next/dist/server/request/headers";

import { auth } from "@/auth";

/**
 * @brief Ensures current request is associated with an authenticated user.
 * @description Retrieves session from request headers and throws a contextual error when user is missing.
 * @param message Action description used in failure message.
 * @returns Authenticated user object.
 */
export const requireUser = async (message: string) => {
  const session = await auth.api.getSession({ headers: await headers() });

  if (!session?.user?.id) {
    throw new Error(`You must be logged in to ${message}!`);
  }

  return session.user;
};
