"use client";

import { useState } from "react";

import { type ViewerSelection } from "./job-viewer-dialog";

export type JobViewerState = {
  open: boolean;
  title: string;
  originalSrc: string;
  selectedSrc: string;
  selection: ViewerSelection;
};

type OpenViewerArgs = Omit<JobViewerState, "open">;

export const useJobViewer = () => {
  const [state, setState] = useState<JobViewerState>({
    open: false,
    title: "",
    originalSrc: "",
    selectedSrc: "",
    selection: "original"
  });

  const openViewer = (args: OpenViewerArgs) => {
    setState({ open: true, ...args });
  };

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
