"use client";

import { useState } from "react";

type FeedbackType = "success" | "error";

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
