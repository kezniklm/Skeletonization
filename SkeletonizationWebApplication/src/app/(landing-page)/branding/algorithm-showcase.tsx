/**
 * @file algorithm-showcase.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Displays supported algorithm badge list.
 * @description Presents available skeletonization algorithms as compact visual tags on landing page.
 * @version 1.0
 * @date 2026-03-16
 */

import { Workflow } from "lucide-react";

import { ALGORITHMS } from "@/algorithms";

/**
 * @brief Renders supported algorithm showcase section.
 * @returns Algorithm badge section JSX.
 */
export const AlgorithmShowcase = () => (
  <section
    className="mt-8 rounded-2xl bg-white/60 p-6 backdrop-blur-sm xl:mt-6 xl:p-5 2xl:mt-8 2xl:p-6 dark:bg-gray-800/60"
    aria-labelledby="algorithms-heading"
  >
    <div className="mb-4 flex items-center space-x-2 xl:space-x-1.5 2xl:space-x-2">
      <Workflow className="h-5 w-5 text-cyan-600 xl:h-4 xl:w-4 2xl:h-5 2xl:w-5 dark:text-cyan-400" aria-hidden="true" />
      <h3 id="algorithms-heading" className="font-semibold text-gray-900 xl:text-sm 2xl:text-base dark:text-white">
        Supported Algorithms
      </h3>
    </div>
    <div
      className="flex flex-wrap gap-2 xl:gap-1.5 2xl:gap-2"
      role="list"
      aria-label="List of supported skeletonization algorithms"
    >
      {ALGORITHMS.map((algo) => (
        <span
          key={algo}
          role="listitem"
          className="rounded-lg bg-linear-to-r from-cyan-600 to-blue-600 px-3 py-1.5 text-xs font-medium text-white shadow-sm xl:px-2.5 xl:py-1 2xl:px-3 2xl:py-1.5"
        >
          {algo}
        </span>
      ))}
    </div>
  </section>
);
