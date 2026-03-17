/**
 * @file use-save-confirmation-dialog.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Manages save confirmation dialog state.
 * @description Tracks dialog visibility and selected follow-up action after saving a preprocessed image.
 */

"use client";

import { useState } from "react";

/**
 * @brief Enumerates follow-up actions available after saving.
 */
export type SaveAction = "continue" | "images" | "skeletonization" | null;

/**
 * @brief Provides state and handlers for the save confirmation dialog.
 * @description Exposes open/close/select actions for post-save user flow control.
 * @returns Save confirmation dialog state and action handlers.
 */
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
