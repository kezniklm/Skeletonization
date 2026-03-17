/**
 * @file preprocessing-workspace.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Renders the main preprocessing workspace layout.
 * @description Connects workspace state hooks with preview and controls panels, including feedback and save confirmation dialogs.
 */

import { type CV } from "mirada/dist/src/types/opencv";
import {
  AlertDialog,
  AlertDialogContent,
  AlertDialogTitle,
  AlertDialogDescription,
  AlertDialogAction
} from "@radix-ui/react-alert-dialog";

import { type SelectImage } from "@/database/zod/image";
import { AlertDialogHeader, AlertDialogFooter } from "@/components/ui/alert-dialog";
import { type FileOutputFormat } from "@/database/zod";

import { usePreprocessingWorkspace } from "./(hooks)/use-preprocessing-workspace";
import { ControlsPanel } from "./(components)/controls-panel";
import { PreviewPanel } from "./(components)/preview-panel";
import { SaveConfirmationDialog } from "./(components)/save-confirmation-dialog";

type PreprocessingWorkspaceProps = {
  selectedImage: SelectImage;
  originalImage: HTMLImageElement;
  cv: CV;
  defaultOutputFormat: FileOutputFormat;
};

/**
 * @brief Displays interactive preprocessing controls and preview for a selected image.
 * @description Binds preprocessing state management to UI panels and modal dialogs for user feedback and save decisions.
 * @param selectedImage Metadata for the source image being edited.
 * @param originalImage Loaded image element used for rendering and comparisons.
 * @param cv OpenCV runtime instance used for processing operations.
 * @param defaultOutputFormat Preferred file format used when exporting output.
 * @returns A workspace view containing preview and controls regions.
 */
export const PreprocessingWorkspace = ({
  selectedImage,
  originalImage,
  cv,
  defaultOutputFormat
}: PreprocessingWorkspaceProps) => {
  const { previewProps, controlsProps, feedbackDialogProps, saveConfirmationDialogProps } = usePreprocessingWorkspace({
    selectedImage,
    cv,
    originalImage,
    defaultOutputFormat
  });

  const { feedbackOpen, setFeedbackOpen, feedbackMessage, feedbackType } = feedbackDialogProps;
  const { isOpen, closeDialog, selectAction } = saveConfirmationDialogProps;

  return (
    <div className="relative z-10 mx-auto max-w-7xl px-4 pb-4 sm:px-6 lg:px-0">
      <AlertDialog open={feedbackOpen} onOpenChange={setFeedbackOpen}>
        <AlertDialogContent>
          <AlertDialogHeader>
            <AlertDialogTitle>{feedbackType === "success" ? "Success" : "Error"}</AlertDialogTitle>
            <AlertDialogDescription>{feedbackMessage}</AlertDialogDescription>
          </AlertDialogHeader>
          <AlertDialogFooter>
            <AlertDialogAction onClick={() => setFeedbackOpen(false)}>OK</AlertDialogAction>
          </AlertDialogFooter>
        </AlertDialogContent>
      </AlertDialog>

      <SaveConfirmationDialog isOpen={isOpen} onClose={closeDialog} onSelectAction={selectAction} />

      <div
        className="grid gap-4 lg:grid-cols-[minmax(0,1fr)_420px] xl:grid-cols-[minmax(0,1fr)_480px]"
        style={{ height: "calc(100vh - 140px)" }}
      >
        <PreviewPanel {...previewProps} />
        <ControlsPanel {...controlsProps} />
      </div>
    </div>
  );
};
