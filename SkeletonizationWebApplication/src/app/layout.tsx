/**
 * @file layout.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Defines root application layout for Next.js app router.
 * @description Initializes session-driven providers, theme preferences, and conditional private/public layout wrappers.
 * @version 1.0
 * @date 2026-03-16
 */

import "./globals.css";
import { headers } from "next/headers";
import { type Metadata } from "next";

import { Providers } from "@/components/layout/providers";
import { PrivateLayout } from "@/components/layout/private-layout";
import { PublicLayout } from "@/components/layout/public-layout";
import { SmallScreenOverlay } from "@/components/layout/small-screen-overlay";
import { auth } from "@/auth";
import { getOrCreateUserPreferences } from "@/repositories/preferences";
import { OpenCvScript } from "@/scripts/opencv";
import { defaultPreferences } from "@/database/schema";

/**
 * @brief Defines static metadata for the application shell.
 * @description Provides default document title for all pages.
 */
export const metadata: Metadata = {
  title: "Skeletonization Web Application"
};

/**
 * @brief Renders root layout with authenticated or public shell.
 * @param children Nested route content.
 * @returns Root HTML document structure with providers.
 */
const RootLayout = async ({
  children
}: Readonly<{
  children: React.ReactNode;
}>) => {
  const session = await auth.api.getSession({ headers: await headers() });

  const isLoggedIn = session?.user ? true : false;

  const userPreferences = session?.user ? await getOrCreateUserPreferences(session.user.id) : null;

  return (
    <html lang="en">
      <head>
        <OpenCvScript />
      </head>
      <body className="flex min-h-full flex-col bg-gray-50 dark:bg-gray-950">
        <Providers
          initialTheme={userPreferences?.theme ?? defaultPreferences.theme}
          animatedBackgroundDisabled={
            userPreferences?.animatedBackgroundDisabled ?? defaultPreferences.animatedBackgroundDisabled
          }
          timezone={userPreferences?.timezone ?? defaultPreferences.timezone}
          isLoggedIn={isLoggedIn}
        >
          <SmallScreenOverlay />
          {isLoggedIn ? <PrivateLayout>{children}</PrivateLayout> : <PublicLayout>{children}</PublicLayout>}
        </Providers>
      </body>
    </html>
  );
};

export default RootLayout;
