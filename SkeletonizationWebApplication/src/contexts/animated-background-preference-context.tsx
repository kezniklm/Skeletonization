"use client";

import { createContext, useContext, useState, type PropsWithChildren } from "react";

type AnimatedBackgroundPreferenceContextType = {
  animatedBackgroundDisabled: boolean;
  setAnimatedBackgroundDisabled: (value: boolean) => void;
};

const AnimatedBackgroundPreferenceContext = createContext<AnimatedBackgroundPreferenceContextType | undefined>(
  undefined
);

type AnimatedBackgroundPreferenceProviderProps = PropsWithChildren<{
  initialAnimatedBackgroundDisabled?: boolean;
}>;

export const AnimatedBackgroundPreferenceProvider = ({
  children,
  initialAnimatedBackgroundDisabled = false
}: AnimatedBackgroundPreferenceProviderProps) => {
  const [animatedBackgroundDisabled, setAnimatedBackgroundDisabled] = useState(initialAnimatedBackgroundDisabled);

  return (
    <AnimatedBackgroundPreferenceContext.Provider value={{ animatedBackgroundDisabled, setAnimatedBackgroundDisabled }}>
      {children}
    </AnimatedBackgroundPreferenceContext.Provider>
  );
};

export const useAnimatedBackgroundPreference = () => {
  const context = useContext(AnimatedBackgroundPreferenceContext);

  if (!context) {
    throw new Error("useAnimatedBackgroundPreference must be used within a AnimatedBackgroundPreferenceProvider");
  }

  return context;
};
