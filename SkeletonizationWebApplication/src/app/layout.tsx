import "./globals.css";
import { headers } from "next/headers";
import { type Metadata } from "next";

import { Providers } from "@/components/layout/providers";
import { PrivateLayout } from "@/components/layout/private-layout";
import { PublicLayout } from "@/components/layout/public-layout";
import { auth } from "@/auth";

export const metadata: Metadata = {
  title: "Skeletonization Web Application"
};

const RootLayout = async ({
  children
}: Readonly<{
  children: React.ReactNode;
}>) => {
  const session = await auth.api.getSession({ headers: await headers() });

  const isLoggedIn = session?.user ? true : false;

  return (
    <html lang="en">
      <body className="flex min-h-full flex-col bg-gray-50 dark:bg-gray-950">
        <Providers>
          {isLoggedIn ? <PrivateLayout>{children}</PrivateLayout> : <PublicLayout>{children}</PublicLayout>}
        </Providers>
      </body>
    </html>
  );
};

export default RootLayout;
