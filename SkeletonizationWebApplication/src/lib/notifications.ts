"use client";

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

export const showSkeletonizationCompleteNotification = (runName: string, imageCount: number) => {
  showNotification("Skeletonization Complete", {
    body: `Run "${runName}" completed successfully for ${imageCount} image${imageCount > 1 ? "s" : ""}.`,
    tag: "skeletonization-complete",
    requireInteraction: false
  });
};

export const showSkeletonizationErrorNotification = (runName: string, error: string) => {
  showNotification("Skeletonization Failed", {
    body: `Run "${runName}" failed: ${error}`,
    tag: "skeletonization-error",
    requireInteraction: true
  });
};
