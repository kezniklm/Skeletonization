import { createAuthClient } from "better-auth/react";

const base = process.env.NEXT_PUBLIC_BASE_URL;
export const authClient = createAuthClient({
  baseURL: base ? `${base}/api/auth` : undefined
});
