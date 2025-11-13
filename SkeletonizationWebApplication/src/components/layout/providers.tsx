"use client";

import { type PropsWithChildren } from "react";
import { Toaster } from "sonner";

import { AnimatedBackgroundProvider } from "./animated-background";

export const Providers = ({ children }: PropsWithChildren) => (
  <AnimatedBackgroundProvider>
    {children}
    <Toaster richColors />
  </AnimatedBackgroundProvider>
);
