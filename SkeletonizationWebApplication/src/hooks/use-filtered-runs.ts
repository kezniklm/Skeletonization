/**
 * @file use-filtered-runs.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Filters and sorts lab runs by query and algorithm.
 * @description Provides run list filtering, sorting, and algorithm toggle state for lab run exploration.
 * @version 1.0
 * @date 2026-03-16
 */

"use client";

import { useState } from "react";

import { type Algorithm } from "@/algorithms";
import { type SortOption } from "@/components/filters";
import { type LabRun } from "@/repositories/run";

const getRunDisplayName = (run: LabRun) => run.name?.trim() ?? `Run ${run.id.slice(0, 8)}`;

const stringifyRunForSearch = (run: LabRun) => {
  const parts = [run.id, run.status, getRunDisplayName(run)];
  for (const j of run.jobs) {
    parts.push(j.algorithm, j.status, j.image.name);
  }
  return parts.join("\n").toLowerCase();
};

const getCreatedAtMs = (run: LabRun) => {
  if (!run.createdAt) return 0;
  const ms = Date.parse(run.createdAt);
  return Number.isFinite(ms) ? ms : 0;
};

const compareBySort = (a: LabRun, b: LabRun, sortBy: SortOption) => {
  switch (sortBy) {
    case "date-asc":
      return getCreatedAtMs(a) - getCreatedAtMs(b) || a.id.localeCompare(b.id);
    case "date-desc":
      return getCreatedAtMs(b) - getCreatedAtMs(a) || a.id.localeCompare(b.id);
    case "name-asc":
      return (
        getRunDisplayName(a).localeCompare(getRunDisplayName(b), undefined, { sensitivity: "base" }) ||
        a.id.localeCompare(b.id)
      );
    case "name-desc":
      return (
        getRunDisplayName(b).localeCompare(getRunDisplayName(a), undefined, { sensitivity: "base" }) ||
        a.id.localeCompare(b.id)
      );
  }
};

/**
 * @brief Returns filtered and sorted run list state.
 * @description Manages search query, sort order, selected algorithms, and derives matching sorted runs.
 * @param runs Source runs collection.
 * @returns Filtering state, actions, and sorted run output.
 */
export const useFilteredRuns = (runs: LabRun[]) => {
  const [searchQuery, setSearchQuery] = useState<string>("");
  const [sortBy, setSortBy] = useState<SortOption>("date-desc");
  const [selectedAlgorithms, setSelectedAlgorithms] = useState<Set<Algorithm>>(() => new Set());

  const normalizedQuery = searchQuery.trim().toLowerCase();
  const activeFilterCount = (normalizedQuery ? 1 : 0) + selectedAlgorithms.size;

  const filteredRuns = runs.filter((run) => {
    const algorithmOk = selectedAlgorithms.size === 0 || run.jobs.some((j) => selectedAlgorithms.has(j.algorithm));
    if (!algorithmOk) return false;
    if (!normalizedQuery) return true;
    return stringifyRunForSearch(run).includes(normalizedQuery);
  });

  const sortedRuns = filteredRuns.slice().sort((a, b) => compareBySort(a, b, sortBy));

  const clearAllFilters = () => {
    setSearchQuery("");
    setSelectedAlgorithms(new Set());
  };

  const toggleAlgorithm = (algorithm: Algorithm) => {
    setSelectedAlgorithms((prev) => {
      const next = new Set(prev);
      if (next.has(algorithm)) {
        next.delete(algorithm);
      } else {
        next.add(algorithm);
      }
      return next;
    });
  };

  return {
    searchQuery,
    setSearchQuery,
    sortBy,
    setSortBy,
    selectedAlgorithms,
    activeFilterCount,
    sortedRuns,
    clearAllFilters,
    toggleAlgorithm
  };
};
