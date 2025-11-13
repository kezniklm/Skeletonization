import { Logo } from "../logo/logo";

import { NavigationLink } from "./navigation-link";

type NavigationLogoProps = {
  className?: string;
};

export const NavigationLogo = ({ className }: NavigationLogoProps) => (
  <div className={className}>
    <NavigationLink
      href="/"
      className="group flex h-10 w-10 items-center justify-center rounded-lg bg-gradient-to-br from-cyan-600 to-blue-600 shadow-md transition-all hover:scale-105 hover:shadow-lg"
    >
      <Logo />
    </NavigationLink>
  </div>
);
