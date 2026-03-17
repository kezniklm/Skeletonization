/**
 * @file social-sign-in-button.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Implements reusable social sign-in action button.
 * @description Triggers OAuth login flow for configured provider with visual variant styling.
 * @version 1.0
 * @date 2026-03-16
 */

"use client";

import type { ReactNode } from "react";

import { authClient } from "@/auth-client";

/**
 * @brief Enumerates supported social authentication providers.
 */
export type SocialProviderId = "google" | "github" | "discord";

/**
 * @brief Defines styling variants for social sign-in buttons.
 */
type SocialSignInButtonVariant = "blue" | "gray";

/**
 * @brief Maps button variant to Tailwind interaction classes.
 * @description Ensures provider buttons share structure while using variant-specific accents.
 */
const VARIANT_CLASSES: Record<SocialSignInButtonVariant, string> = {
  blue: "hover:border-blue-500 focus:ring-blue-500/50 dark:hover:border-blue-400",
  gray: "hover:border-gray-500 focus:ring-gray-500/50 dark:hover:border-gray-400"
};

/**
 * @brief Represents social sign-in button properties.
 */
type SocialSignInButtonProps = Readonly<{
  provider: SocialProviderId;
  label: string;
  icon: ReactNode;
  variant: SocialSignInButtonVariant;
}>;

/**
 * @brief Renders social sign-in button for a specific provider.
 * @param provider Authentication provider identifier.
 * @param label Provider display label.
 * @param icon Provider icon element.
 * @param variant Visual style variant.
 * @returns Provider sign-in button JSX.
 */
export const SocialSignInButton = ({ provider, label, icon, variant }: SocialSignInButtonProps) => {
  /**
   * @brief Starts OAuth social sign-in flow.
   * @returns Promise resolved after auth client call is dispatched.
   */
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
