/**
 * @file notification-permission-dialog.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Dialog for requesting browser notification permission.
 * @description Prompts users to grant notification access when preferences are enabled but browser permission is missing.
 * @version 1.0
 * @date 2026-03-16
 */

"use client";

import { useState } from "react";
import { Bell } from "lucide-react";
import { toast } from "sonner";

import {
  AlertDialog,
  AlertDialogAction,
  AlertDialogCancel,
  AlertDialogContent,
  AlertDialogDescription,
  AlertDialogFooter,
  AlertDialogHeader,
  AlertDialogTitle
} from "@/components/ui/alert-dialog";
import { requestNotificationPermission } from "@/lib/notifications";

type NotificationPermissionDialogProps = {
  notificationsEnabled: boolean;
};

/**
 * @brief Prompts for browser notification permission when needed.
 */
export const NotificationPermissionDialog = ({ notificationsEnabled }: NotificationPermissionDialogProps) => {
  const [dismissed, setDismissed] = useState(false);

  const isMissingBrowserPermission =
    typeof window !== "undefined" && "Notification" in window && Notification.permission !== "granted";

  const open = notificationsEnabled && isMissingBrowserPermission && !dismissed;

  const handleOpenChange = (isOpen: boolean) => {
    if (!isOpen) {
      setDismissed(true);
    }
  };

  const handleRequestNotifications = async () => {
    const permission = await requestNotificationPermission();

    if (permission === "granted") {
      toast.success("Notifications enabled");
    } else {
      toast.error("Notification permission denied");
    }

    setDismissed(true);
  };

  const handleDeclineNotifications = () => {
    setDismissed(true);
  };

  return (
    <AlertDialog open={open} onOpenChange={handleOpenChange}>
      <AlertDialogContent>
        <AlertDialogHeader>
          <AlertDialogTitle className="flex items-center gap-2">
            <Bell className="h-5 w-5 text-cyan-600 dark:text-cyan-400" />
            Enable Browser Notifications?
          </AlertDialogTitle>
          <AlertDialogDescription>
            You have notifications enabled in your preferences, but browser notifications are not yet granted. Would you
            like to enable browser notifications to get real-time updates when your skeletonization runs complete?
          </AlertDialogDescription>
        </AlertDialogHeader>

        <AlertDialogFooter>
          <AlertDialogCancel onClick={handleDeclineNotifications}>Not Now</AlertDialogCancel>
          <AlertDialogAction onClick={handleRequestNotifications}>Enable Notifications</AlertDialogAction>
        </AlertDialogFooter>
      </AlertDialogContent>
    </AlertDialog>
  );
};
