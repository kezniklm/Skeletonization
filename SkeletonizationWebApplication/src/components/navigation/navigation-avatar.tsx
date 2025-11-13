"use client";

import { useState } from "react";
import Image from "next/image";
import { User as UserIcon } from "lucide-react";

const NavigationAvatar = ({ src, alt }: { src?: string; alt?: string }) => {
  const [error, setError] = useState(false);

  if (!src || error) {
    return (
      <div className="flex h-8 w-8 items-center justify-center rounded-full bg-gradient-to-br from-cyan-500 to-blue-500">
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
