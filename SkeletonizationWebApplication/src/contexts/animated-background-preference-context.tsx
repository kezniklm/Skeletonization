/**
 * @file animated-background-preference-context.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Stores animated background preference state.
 * @description Provides context and hook helpers for enabling or disabling animated background visuals.
 * @version 1.0
 * @date 2026-03-16
 */

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

/**
 * @brief Provides animated background preference context.
 * @description Wraps descendants with mutable state representing whether animated background is disabled.
 * @param children Descendant nodes consuming background preference.
 * @param initialAnimatedBackgroundDisabled Initial disabled value.
 * @returns Context provider wrapper.
 */
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

/**
 * @brief Returns animated background preference context value.
 * @description Throws an error when called outside the corresponding provider.
 * @returns Background preference state and setter.
 */
export const useAnimatedBackgroundPreference = () => {
  const context = useContext(AnimatedBackgroundPreferenceContext);

  if (!context) {
    throw new Error("useAnimatedBackgroundPreference must be used within a AnimatedBackgroundPreferenceProvider");
  }

  return context;
};
