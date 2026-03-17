/**
 * @file navigation-link.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Wraps next/link for navigation items.
 * @description Provides a typed reusable navigation link component for desktop and mobile menus.
 * @version 1.0
 * @date 2026-03-16
 */

import Link from "next/link";

type NavigationLinkProps = {
  href: string;
  className: string;
  onClick?: () => void;
};

/**
 * @brief Renders a typed navigation link wrapper.
 * @description Forwards href, styling, and optional click callback to Next.js link.
 * @param href Target route path.
 * @param children Link content.
 * @param className Classes applied to the link.
 * @param onClick Optional click callback.
 * @returns A Next.js link element.
 */
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
