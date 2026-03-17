/**
 * @file notifications.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Browser notification utility helpers.
 * @description Handles permission requests and standardized run-completion/error notification payloads.
 * @version 1.0
 * @date 2026-03-16
 */

"use client";

/** @brief Requests browser notification permission when available. */
export const requestNotificationPermission = async (): Promise<NotificationPermission> => {
  if (!("Notification" in window)) {
    console.warn("This browser does not support notifications");
    return "denied";
  }

  if (Notification.permission === "granted") {
    return "granted";
  }

  if (Notification.permission !== "denied") {
    const permission = await Notification.requestPermission();

    return permission;
  }

  return Notification.permission;
};

/** @brief Displays a browser notification when permission is granted. */
export const showNotification = (title: string, options?: NotificationOptions) => {
  if (!("Notification" in window)) {
    console.warn("This browser does not support notifications");
    return;
  }

  if (Notification.permission === "granted") {
    new Notification(title, {
      icon: "/favicon.ico",
      badge: "/favicon.ico",
      ...options
    });
  }
};

/** @brief Shows standardized success notification for completed run. */
export const showSkeletonizationCompleteNotification = (runName: string, imageCount: number) => {
  showNotification("Skeletonization Complete", {
    body: `Run "${runName}" completed successfully for ${imageCount} image${imageCount > 1 ? "s" : ""}.`,
    tag: "skeletonization-complete",
    requireInteraction: false
  });
};

/** @brief Shows standardized error notification for failed run. */
export const showSkeletonizationErrorNotification = (runName: string, error: string) => {
  showNotification("Skeletonization Failed", {
    body: `Run "${runName}" failed: ${error}`,
    tag: "skeletonization-error",
    requireInteraction: true
  });
};
