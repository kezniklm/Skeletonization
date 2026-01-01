"use client";

import { type LabRun } from "@/repositories/run";
import { ALGORITHMS } from "@/algorithms";

import { useFilteredRuns } from "../../hooks/use-filtered-runs";

import { RunDetails } from "./run-details";
import { LabFilters } from "./lab-filters";
import { LabAlgorithmFilter } from "./lab-algorithm-filter";
import { LabSort } from "./lab-sort";

export const LabHistoryClient = ({ runs, totalRuns }: { runs: LabRun[]; totalRuns: number }) => {
  const {
    searchQuery,
    setSearchQuery,
    sortBy,
    setSortBy,
    selectedAlgorithms,
    activeFilterCount,
    sortedRuns,
    clearAllFilters,
    toggleAlgorithm
  } = useFilteredRuns(runs);

  return (
    <div className="overflow-hidden rounded-xl border border-gray-200 bg-white shadow-sm backdrop-blur-sm xl:rounded-lg 2xl:rounded-xl dark:border-gray-800 dark:bg-gray-900/95">
      <div className="border-b border-gray-200 bg-linear-to-r from-cyan-50/50 to-blue-50/50 px-6 py-4 xl:px-5 xl:py-3 2xl:px-6 2xl:py-4 dark:border-gray-800 dark:from-cyan-950/20 dark:to-blue-950/20">
        <div className="flex flex-col gap-3">
          <div>
            <h2 className="text-lg font-semibold text-gray-900 xl:text-base 2xl:text-lg dark:text-white">
              Processing History
            </h2>
            <p className="mt-1 text-sm text-gray-600 xl:text-xs 2xl:text-sm dark:text-gray-400">
              {totalRuns} total run{totalRuns !== 1 ? "s" : ""}
            </p>
          </div>

          <LabFilters
            searchQuery={searchQuery}
            onSearchChange={setSearchQuery}
            activeFilterCount={activeFilterCount}
            clearAllFilters={clearAllFilters}
            sortControl={
              <>
                <LabSort sortBy={sortBy} onSortChange={setSortBy} />
                <LabAlgorithmFilter
                  algorithms={ALGORITHMS}
                  selectedAlgorithms={selectedAlgorithms}
                  toggleAlgorithm={toggleAlgorithm}
                />
              </>
            }
          />
        </div>
      </div>

      <div className="divide-y divide-gray-200 dark:divide-gray-800">
        {sortedRuns.length === 0 ? (
          <div className="px-6 py-10 text-sm text-gray-600 xl:px-5 dark:text-gray-400">No runs match your search.</div>
        ) : (
          sortedRuns.map((run, index) => (
            <RunDetails
              key={run.id}
              run={run}
              className={index === sortedRuns.length - 1 ? "rounded-b-xl xl:rounded-b-lg 2xl:rounded-b-xl" : undefined}
            />
          ))
        )}
      </div>
    </div>
  );
};
