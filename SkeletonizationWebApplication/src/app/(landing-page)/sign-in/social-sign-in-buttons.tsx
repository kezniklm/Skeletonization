import type { ReactNode } from "react";

import { SocialSignInButton, type SocialProviderId } from "./social-sign-in-button";
import { DiscordIcon, GitHubIcon, GoogleIcon } from "./social-sign-in-icons";

type SocialButtonConfig = Readonly<{
  provider: SocialProviderId;
  label: string;
  variant: "blue" | "gray";
  icon: ReactNode;
}>;

const ICON_CLASS_NAME = "h-6 w-6 transition-transform group-hover:scale-110 xl:h-5 xl:w-5 2xl:h-6 2xl:w-6";

const SOCIAL_SIGN_IN_BUTTONS: readonly SocialButtonConfig[] = [
  {
    provider: "google",
    label: "Google",
    variant: "blue",
    icon: <GoogleIcon className={ICON_CLASS_NAME} />
  },
  {
    provider: "github",
    label: "GitHub",
    variant: "gray",
    icon: <GitHubIcon className={ICON_CLASS_NAME} />
  },
  {
    provider: "discord",
    label: "Discord",
    variant: "gray",
    icon: <DiscordIcon className={ICON_CLASS_NAME} />
  }
];

export const SocialSignInButtons = () => (
  <>
    {SOCIAL_SIGN_IN_BUTTONS.map(({ provider, label, icon, variant }) => (
      <SocialSignInButton key={provider} provider={provider} label={label} icon={icon} variant={variant} />
    ))}
  </>
);
