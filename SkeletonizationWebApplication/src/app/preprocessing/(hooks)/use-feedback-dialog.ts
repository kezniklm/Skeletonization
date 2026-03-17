/**
 * @file use-feedback-dialog.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Provides feedback dialog state for preprocessing actions.
 * @description Encapsulates message, type, and visibility state used to present success and error notifications.
 */

"use client";

import { useState } from "react";

type FeedbackType = "success" | "error";

/**
 * @brief Manages feedback dialog visibility and message state.
 * @description Exposes current dialog state and a helper to show typed feedback messages.
 * @returns Feedback dialog state values and control handlers.
 */
export const useFeedbackDialog = () => {
  const [feedbackOpen, setFeedbackOpen] = useState(false);
  const [feedbackMessage, setFeedbackMessage] = useState("");
  const [feedbackType, setFeedbackType] = useState<FeedbackType>("success");

  const showFeedback = (message: string, type: FeedbackType = "success") => {
    setFeedbackMessage(message);
    setFeedbackType(type);
    setFeedbackOpen(true);
  };

  return {
    feedbackOpen,
    setFeedbackOpen,
    feedbackMessage,
    feedbackType,
    showFeedback
  };
};
