/**
 * @file navigation-avatar.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Renders user avatar with fallback icon.
 * @description Displays user image when available and falls back to generated icon badge on missing or failed image load.
 * @version 1.0
 * @date 2026-03-16
 */

"use client";

import { useState } from "react";
import Image from "next/image";
import { User as UserIcon } from "lucide-react";

/**
 * @brief Displays a navigation avatar image with fallback.
 * @param src Optional avatar source URL.
 * @param alt Optional avatar alt text.
 * @returns Avatar image or fallback icon.
 */
const NavigationAvatar = ({ src, alt }: { src?: string; alt?: string }) => {
  const [error, setError] = useState(false);

  if (!src || error) {
    return (
      <div className="flex h-8 w-8 items-center justify-center rounded-full bg-linear-to-br from-cyan-500 to-blue-500">
        <UserIcon className="h-5 w-5 text-white" />
      </div>
    );
  }

  return (
    <Image
      src={src}
      alt={alt ?? "User avatar"}
      width={32}
      height={32}
      className="h-full w-full rounded-full object-cover ring-2 ring-cyan-500 ring-offset-2"
      onError={() => setError(true)}
    />
  );
};

export default NavigationAvatar;
