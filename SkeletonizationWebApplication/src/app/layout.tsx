"use client";

import "./globals.css";
import { redirect } from "next/navigation";
import { type Metadata } from "next/types";

import { auth, authClient } from "../auth";

// export const metadata: Metadata = {
//   title: "Skeletonization Web Application"
// };

const RootLayout = ({
  children
}: Readonly<{
  children: React.ReactNode;
}>) => {
  const session = authClient.useSession();
  const isLoggedIn = session?.data?.user ? true : false; //Session verification,...
  if (!isLoggedIn) redirect("/sign-in");
  return <div>Welcome {session?.data?.user.name ?? session?.data?.user.email}</div>;
};

export default RootLayout;
