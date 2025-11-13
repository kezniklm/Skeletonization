import Link from "next/link";

type NavigationLinkProps = {
  href: string;
  className: string;
  onClick?: () => void;
};

export const NavigationLink = ({
  href,
  children,
  className,
  onClick
}: React.PropsWithChildren<NavigationLinkProps>) => (
  <Link href={href} className={className} onClick={onClick}>
    {children}
  </Link>
);
