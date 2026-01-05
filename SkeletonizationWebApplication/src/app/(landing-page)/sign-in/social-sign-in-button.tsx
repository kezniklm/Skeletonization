"use client";

import type { ReactNode } from "react";

import { authClient } from "@/auth-client";

export type SocialProviderId = "google" | "github" | "discord";

type SocialSignInButtonVariant = "blue" | "gray";

const VARIANT_CLASSES: Record<SocialSignInButtonVariant, string> = {
  blue: "hover:border-blue-500 focus:ring-blue-500/50 dark:hover:border-blue-400",
  gray: "hover:border-gray-500 focus:ring-gray-500/50 dark:hover:border-gray-400"
};

type SocialSignInButtonProps = Readonly<{
  provider: SocialProviderId;
  label: string;
  icon: ReactNode;
  variant: SocialSignInButtonVariant;
}>;

export const SocialSignInButton = ({ provider, label, icon, variant }: SocialSignInButtonProps) => {
  const handleSignIn = async () => {
    await authClient.signIn.social({
      provider,
      callbackURL: "/"
    });
  };

  return (
    <button
      onClick={handleSignIn}
      className={`group relative flex w-full items-center justify-center gap-3 overflow-hidden rounded-xl border-2 border-gray-200 bg-white px-6 py-4 text-base font-semibold text-gray-700 shadow-lg transition-all hover:shadow-xl focus:ring-4 focus:outline-none xl:gap-2 xl:px-5 xl:py-3 xl:text-sm xl:shadow-md 2xl:gap-3 2xl:px-6 2xl:py-4 2xl:text-base 2xl:shadow-lg dark:border-gray-700 dark:bg-gray-900 dark:text-gray-200 ${VARIANT_CLASSES[variant]}`}
      aria-label={`Sign in with ${label}`}
      type="button"
    >
      {icon}
      <span>{`Continue with ${label}`}</span>
    </button>
  );
};
