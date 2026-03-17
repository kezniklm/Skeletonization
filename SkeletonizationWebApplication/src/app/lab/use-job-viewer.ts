/**
 * @file use-job-viewer.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Manages state for lab job image viewer dialog.
 * @description Provides open/close handlers and selected image payload for viewer modal.
 * @version 1.0
 * @date 2026-03-16
 */

"use client";

import { useState } from "react";

import { type ViewerSelection } from "./job-viewer-dialog";

/**
 * @brief Represents complete viewer dialog state.
 */
export type JobViewerState = {
  open: boolean;
  title: string;
  originalSrc: string;
  selectedSrc: string;
  selection: ViewerSelection;
};

/**
 * @brief Represents parameters needed to open viewer.
 */
type OpenViewerArgs = Omit<JobViewerState, "open">;

/**
 * @brief Provides viewer state and control handlers.
 * @returns Viewer state selectors and open/close callbacks.
 */
export const useJobViewer = () => {
  const [state, setState] = useState<JobViewerState>({
    open: false,
    title: "",
    originalSrc: "",
    selectedSrc: "",
    selection: "original"
  });

  /**
   * @brief Opens viewer with selected image payload.
   * @param args Viewer image and title arguments.
   * @returns No return value.
   */
  const openViewer = (args: OpenViewerArgs) => {
    setState({ open: true, ...args });
  };

  /**
   * @brief Closes viewer while preserving latest payload.
   * @returns No return value.
   */
  const closeViewer = () => {
    setState((prev) => ({ ...prev, open: false }));
  };

  return {
    viewerOpen: state.open,
    viewerTitle: state.title,
    viewerOriginalSrc: state.originalSrc,
    viewerSelectedSrc: state.selectedSrc,
    viewerSelection: state.selection,
    openViewer,
    closeViewer
  };
};
