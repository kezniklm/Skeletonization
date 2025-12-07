"use client";

import { useState } from "react";

export type SaveAction = "continue" | "images" | "skeletonization" | null;

export const useSaveConfirmationDialog = () => {
  const [isOpen, setIsOpen] = useState(false);
  const [selectedAction, setSelectedAction] = useState<SaveAction>(null);

  const openDialog = () => {
    setIsOpen(true);
  };

  const closeDialog = () => {
    setIsOpen(false);
    setSelectedAction(null);
  };

  const selectAction = (action: SaveAction) => {
    setSelectedAction(action);
    setIsOpen(false);
  };

  return {
    isOpen,
    openDialog,
    closeDialog,
    selectAction,
    selectedAction
  };
};
