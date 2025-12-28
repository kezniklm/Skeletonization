"use client";

import { useState } from "react";
import { ChevronDown, ChevronRight, ImageIcon, Sparkles, ZoomIn } from "lucide-react";
import Image from "next/image";

import { type Algorithm } from "@/algorithms";
import { type LabJob, type LabRun } from "@/repositories/run";
import { formatAlgorithmName, formatStatus } from "@/lib/format";
import { cn } from "@/lib/utils";

import { JobViewerDialog } from "./job-viewer-dialog";
import { useJobViewer } from "./use-job-viewer";

const getImageSrc = (storagePath: string) => {
  const normalized = storagePath.replace(/\\/g, "/");
  return normalized.startsWith("/") ? normalized : `/${normalized}`;
};

const getStatusPillClass = (status: string) => {
  switch (status) {
    case "completed":
      return "bg-green-100 text-green-800 dark:bg-green-900/30 dark:text-green-300";
    case "running":
    case "processing":
      return "bg-blue-100 text-blue-800 dark:bg-blue-900/30 dark:text-blue-300";
    case "failed":
      return "bg-red-100 text-red-800 dark:bg-red-900/30 dark:text-red-300";
    case "queued":
      return "bg-yellow-100 text-yellow-800 dark:bg-yellow-900/30 dark:text-yellow-300";
    default:
      return "bg-gray-100 text-gray-800 dark:bg-gray-800 dark:text-gray-300";
  }
};

const getRunDisplayName = (run: Pick<LabRun, "name" | "id">) =>
  run.name?.trim() ? run.name.trim() : `Run ${run.id.slice(0, 8)}`;

const formatDateTime = (iso: string | null) => {
  if (!iso) return null;
  const date = new Date(iso);
  if (Number.isNaN(date.getTime())) return null;
  return new Intl.DateTimeFormat(undefined, {
    year: "numeric",
    month: "short",
    day: "2-digit",
    hour: "2-digit",
    minute: "2-digit"
  }).format(date);
};

const formatDuration = (startIso: string | null, endIso: string | null) => {
  if (!startIso || !endIso) return null;
  const start = new Date(startIso).getTime();
  const end = new Date(endIso).getTime();
  if (!Number.isFinite(start) || !Number.isFinite(end) || end < start) return null;

  const totalSeconds = Math.floor((end - start) / 1000);
  const minutes = Math.floor(totalSeconds / 60);
  const seconds = totalSeconds % 60;

  if (minutes === 0) return `${seconds}s`;
  return `${minutes}m ${seconds}s`;
};

const getUniqueAlgorithms = (jobs: readonly Pick<LabJob, "algorithm">[]) => {
  const seen = new Set<Algorithm>();
  for (const j of jobs) {
    seen.add(j.algorithm);
  }
  return Array.from(seen).sort((a, b) => a.localeCompare(b));
};

type GroupedImageJobs = {
  image: LabJob["image"];
  originalSrc: string;
  minOrdinal: number;
  items: LabJob[];
};

const groupJobsByImage = (jobs: readonly LabJob[]): GroupedImageJobs[] => {
  const imageGroups = new Map<string, GroupedImageJobs>();

  for (const j of jobs) {
    const existing = imageGroups.get(j.image.id);

    if (!existing) {
      imageGroups.set(j.image.id, {
        image: j.image,
        originalSrc: getImageSrc(j.image.storagePath),
        minOrdinal: j.ordinal,
        items: [j]
      });
      continue;
    }

    existing.items.push(j);
    existing.minOrdinal = Math.min(existing.minOrdinal, j.ordinal);
  }

  return Array.from(imageGroups.values()).sort((a, b) => a.minOrdinal - b.minOrdinal);
};

export const RunDetails = ({ run, className }: { run: LabRun; className?: string }) => {
  const [expanded, setExpanded] = useState(false);

  const { viewerOpen, viewerTitle, viewerOriginalSrc, viewerSelectedSrc, viewerSelection, openViewer, closeViewer } =
    useJobViewer();

  const createdLabel = formatDateTime(run.createdAt);
  const durationLabel = formatDuration(run.startedAt, run.completedAt);

  const algorithms = getUniqueAlgorithms(run.jobs);
  const groupedImages = groupJobsByImage(run.jobs);

  return (
    <div className={cn("overflow-hidden bg-white dark:bg-gray-900/95", className)}>
      <button
        type="button"
        onClick={() => setExpanded((v) => !v)}
        className="flex w-full items-center justify-between gap-3 px-6 py-4 text-left xl:px-5 xl:py-3 2xl:px-6 2xl:py-4"
      >
        <div className="min-w-0 flex-1">
          <div className="flex flex-wrap items-center gap-2">
            <h3 className="truncate text-base font-semibold text-gray-900 xl:text-sm 2xl:text-base dark:text-white">
              {getRunDisplayName(run)}
            </h3>
            <span
              className={cn(
                "inline-flex items-center rounded-full px-2 py-0.5 text-xs font-medium",
                getStatusPillClass(run.status)
              )}
            >
              {formatStatus(run.status)}
            </span>
          </div>

          <div className="mt-1 flex flex-wrap items-center gap-x-4 gap-y-1 text-sm text-gray-600 xl:text-xs 2xl:text-sm dark:text-gray-400">
            {createdLabel && <span className="truncate">Created {createdLabel}</span>}
            <span>
              {run.jobs.length} image{run.jobs.length !== 1 ? "s" : ""}
            </span>
            {durationLabel && <span>Duration {durationLabel}</span>}
          </div>

          {algorithms.length > 0 && (
            <div className="mt-2 flex flex-wrap gap-1.5">
              {algorithms.map((a) => (
                <span
                  key={a}
                  className="inline-flex items-center gap-1 rounded-full border border-gray-200 bg-gray-50 px-2 py-0.5 text-xs text-gray-700 dark:border-gray-800 dark:bg-gray-900 dark:text-gray-300"
                  title={a}
                >
                  <Sparkles className="h-3.5 w-3.5" />
                  {formatAlgorithmName(a)}
                </span>
              ))}
            </div>
          )}
        </div>

        <div className="shrink-0 text-gray-500 dark:text-gray-400">
          {expanded ? <ChevronDown className="h-5 w-5" /> : <ChevronRight className="h-5 w-5" />}
        </div>
      </button>

      {expanded && (
        <div className="border-t border-gray-200 px-6 py-5 xl:px-5 xl:py-4 2xl:px-6 2xl:py-5 dark:border-gray-800">
          <div className="space-y-6">
            {groupedImages.map((group) => (
              <div key={group.image.id}>
                <p className="truncate text-sm font-semibold text-gray-900 dark:text-white">{group.image.name}</p>
                <p className="mt-1 text-xs text-gray-600 dark:text-gray-400">
                  {group.items.length} output{group.items.length !== 1 ? "s" : ""}
                </p>

                <div className="mt-4 grid auto-cols-[minmax(18rem,1fr)] grid-flow-col gap-3 overflow-x-auto pb-2 sm:auto-cols-[minmax(20rem,1fr)] lg:auto-cols-[minmax(24rem,1fr)]">
                  <button
                    type="button"
                    className="group overflow-hidden rounded-lg border border-gray-200 bg-gray-50 text-left dark:border-gray-800 dark:bg-gray-900"
                    onClick={() => {
                      openViewer({
                        title: group.image.name,
                        originalSrc: group.originalSrc,
                        selectedSrc: group.originalSrc,
                        selection: "original"
                      });
                    }}
                  >
                    <div className="relative aspect-4/3 w-full overflow-hidden bg-gray-100 dark:bg-gray-900">
                      <Image
                        src={group.originalSrc}
                        alt={group.image.name}
                        width={1024}
                        height={768}
                        className="h-full w-full object-contain transition-transform duration-200 group-hover:scale-[1.02]"
                        loading="lazy"
                      />
                      <div className="pointer-events-none absolute inset-0 flex items-center justify-center bg-black/0 opacity-0 transition-opacity duration-200 group-hover:bg-black/20 group-hover:opacity-100">
                        <div className="rounded-full bg-white/90 p-2 text-gray-900 shadow-sm dark:bg-gray-950/80 dark:text-white">
                          <ZoomIn className="h-4 w-4" />
                        </div>
                      </div>
                    </div>
                    <div className="px-3 py-2">
                      <p className="truncate text-xs font-medium text-gray-900 dark:text-white">Original</p>
                      <p className="truncate text-[11px] text-gray-600 dark:text-gray-400">Input image</p>
                    </div>
                  </button>

                  {group.items
                    .slice()
                    .sort((a, b) => a.algorithm.localeCompare(b.algorithm))
                    .map((ri) => {
                      const outputSrc = ri.producedImage ? getImageSrc(ri.producedImage.storagePath) : "";
                      const isClickable = Boolean(outputSrc);

                      return (
                        <button
                          key={ri.id}
                          type="button"
                          disabled={!isClickable}
                          className={cn(
                            "group overflow-hidden rounded-lg border border-gray-200 bg-gray-50 text-left dark:border-gray-800 dark:bg-gray-900",
                            !isClickable && "cursor-not-allowed opacity-60"
                          )}
                          onClick={() => {
                            if (!outputSrc) return;
                            openViewer({
                              title: `${group.image.name} • ${formatAlgorithmName(ri.algorithm)}`,
                              originalSrc: group.originalSrc,
                              selectedSrc: outputSrc,
                              selection: "output"
                            });
                          }}
                        >
                          <div className="relative aspect-4/3 w-full overflow-hidden bg-gray-100 dark:bg-gray-900">
                            {outputSrc ? (
                              <Image
                                src={outputSrc}
                                alt={`${group.image.name} output`}
                                width={1024}
                                height={768}
                                className="h-full w-full object-contain transition-transform duration-200 group-hover:scale-[1.02]"
                                loading="lazy"
                              />
                            ) : (
                              <div className="flex h-full w-full items-center justify-center">
                                <ImageIcon className="h-5 w-5 text-gray-400" />
                              </div>
                            )}

                            {outputSrc && (
                              <div className="pointer-events-none absolute inset-0 flex items-center justify-center bg-black/0 opacity-0 transition-opacity duration-200 group-hover:bg-black/20 group-hover:opacity-100">
                                <div className="rounded-full bg-white/90 p-2 text-gray-900 shadow-sm dark:bg-gray-950/80 dark:text-white">
                                  <ZoomIn className="h-4 w-4" />
                                </div>
                              </div>
                            )}
                          </div>
                          <div className="px-3 py-2">
                            <div className="flex items-center justify-between gap-2">
                              <p className="truncate text-xs font-medium text-gray-900 dark:text-white">
                                {formatAlgorithmName(ri.algorithm)}
                              </p>
                              <span
                                className={cn(
                                  "shrink-0 rounded-full px-2 py-0.5 text-[10px] font-medium",
                                  getStatusPillClass(ri.status)
                                )}
                              >
                                {formatStatus(ri.status)}
                              </span>
                            </div>
                            <p className="truncate text-[11px] text-gray-600 dark:text-gray-400">
                              {ri.producedImage ? "Skeleton" : "No output"}
                            </p>
                          </div>
                        </button>
                      );
                    })}
                </div>
              </div>
            ))}
          </div>

          <JobViewerDialog
            open={viewerOpen}
            onOpenChange={(open) => {
              if (open) return;
              closeViewer();
            }}
            title={viewerTitle}
            originalSrc={viewerOriginalSrc}
            selectedSrc={viewerSelectedSrc}
            selection={viewerSelection}
          />
        </div>
      )}
    </div>
  );
};
