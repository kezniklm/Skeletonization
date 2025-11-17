"use client";

import { type PropsWithChildren } from "react";
import { Toaster } from "sonner";
import { QueryClient, QueryClientProvider } from "@tanstack/react-query";

import { AnimatedBackgroundProvider } from "./animated-background";

const queryClient = new QueryClient();

export const Providers = ({ children }: PropsWithChildren) => (
  <QueryClientProvider client={queryClient}>
    <AnimatedBackgroundProvider>
      {children}
      <Toaster richColors />
    </AnimatedBackgroundProvider>
  </QueryClientProvider>
);
