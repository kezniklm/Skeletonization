import { useRouter } from "next/navigation";

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

import { type SaveAction } from "../(hooks)/use-save-confirmation-dialog";

type SaveConfirmationDialogProps = {
  isOpen: boolean;
  onClose: () => void;
  onSelectAction: (action: SaveAction) => void;
};

export const SaveConfirmationDialog = ({ isOpen, onClose, onSelectAction }: SaveConfirmationDialogProps) => {
  const router = useRouter();

  const handleContinueWork = () => {
    onSelectAction("continue");
    onClose();
  };

  const handleGoToLibrary = () => {
    onSelectAction("images");
    onClose();
    router.push("/images");
  };

  const handleGoToSkeletonization = () => {
    onSelectAction("skeletonization");
    onClose();
    router.push("/skeletonization");
  };

  return (
    <AlertDialog open={isOpen} onOpenChange={onClose}>
      <AlertDialogContent className="max-w-md">
        <AlertDialogHeader>
          <AlertDialogTitle>Image Saved Successfully!</AlertDialogTitle>
          <AlertDialogDescription>
            Your processed image has been saved to the library. What would you like to do next?
          </AlertDialogDescription>
        </AlertDialogHeader>
        <AlertDialogFooter className="flex gap-2 sm:flex-col">
          <AlertDialogCancel onClick={handleContinueWork}>Continue Working</AlertDialogCancel>
          <AlertDialogAction onClick={handleGoToLibrary}>Go to Library</AlertDialogAction>
          <AlertDialogAction onClick={handleGoToSkeletonization}>Go to Skeletonization</AlertDialogAction>
        </AlertDialogFooter>
      </AlertDialogContent>
    </AlertDialog>
  );
};
