import { headers } from "next/dist/server/request/headers";

import { auth } from "@/auth";

export const requireUser = async (message: string) => {
  const session = await auth.api.getSession({ headers: await headers() });

  if (!session?.user?.id) {
    throw new Error(`You must be logged in to ${message}!`);
  }

  return session.user;
};
