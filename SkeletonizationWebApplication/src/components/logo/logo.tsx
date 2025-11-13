import { GitBranch } from "lucide-react";

type LogoProps = {
  className?: string;
};

export const Logo = ({ className }: LogoProps) => <GitBranch className={`h-6 w-6 text-white ${className}`} />;
