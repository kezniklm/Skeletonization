"use client";

import { createContext, useContext, useState, type PropsWithChildren } from "react";

type CompactModeContextType = {
  compactMode: boolean;
  setCompactMode: (value: boolean) => void;
};

const CompactModeContext = createContext<CompactModeContextType | undefined>(undefined);

type CompactModeProviderProps = PropsWithChildren<{
  initialCompactMode?: boolean;
}>;

export const CompactModeProvider = ({ children, initialCompactMode = false }: CompactModeProviderProps) => {
  const [compactMode, setCompactMode] = useState(initialCompactMode);

  return <CompactModeContext.Provider value={{ compactMode, setCompactMode }}>{children}</CompactModeContext.Provider>;
};

export const useCompactMode = () => {
  const context = useContext(CompactModeContext);

  if (!context) {
    throw new Error("useCompactMode must be used within a CompactModeProvider");
  }

  return context;
};
