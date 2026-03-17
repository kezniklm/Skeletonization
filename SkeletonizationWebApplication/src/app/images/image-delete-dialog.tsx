/**
 * @file image-delete-dialog.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Defines confirmation dialog for image deletion.
 * @description Presents destructive confirmation UI before permanently removing an image.
 * @version 1.0
 * @date 2026-03-16
 */

"use client";

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

/**
 * @brief Represents delete dialog control properties.
 */
type ImageDeleteDialogProps = {
  open: boolean;
  onOpenChange: (open: boolean) => void;
  onConfirm: () => void;
};

/**
 * @brief Renders delete confirmation dialog.
 * @param open Controls dialog visibility.
 * @param onOpenChange Callback for dialog open state changes.
 * @param onConfirm Callback fired when deletion is confirmed.
 * @returns Alert dialog JSX.
 */
export const ImageDeleteDialog = ({ open, onOpenChange, onConfirm }: ImageDeleteDialogProps) => (
  <AlertDialog open={open} onOpenChange={onOpenChange}>
    <AlertDialogContent className="max-w-md">
      <AlertDialogHeader>
        <AlertDialogTitle>Delete Image?</AlertDialogTitle>
        <AlertDialogDescription>
          This action cannot be undone. This will permanently delete the image from your library and remove all
          associated data.
        </AlertDialogDescription>
      </AlertDialogHeader>
      <AlertDialogFooter>
        <AlertDialogCancel>Cancel</AlertDialogCancel>
        <AlertDialogAction onClick={onConfirm} className="bg-destructive hover:bg-destructive/90 text-white">
          Delete Image
        </AlertDialogAction>
      </AlertDialogFooter>
    </AlertDialogContent>
  </AlertDialog>
);
