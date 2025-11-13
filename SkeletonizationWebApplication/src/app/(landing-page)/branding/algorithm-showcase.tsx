import { Workflow } from "lucide-react";

import { algorithms } from "@/algorithms";

export const AlgorithmShowcase = () => (
  <section
    className="mt-8 rounded-2xl bg-white/60 p-6 backdrop-blur-sm dark:bg-gray-800/60"
    aria-labelledby="algorithms-heading"
  >
    <div className="mb-4 flex items-center space-x-2">
      <Workflow className="h-5 w-5 text-cyan-600 dark:text-cyan-400" aria-hidden="true" />
      <h3 id="algorithms-heading" className="font-semibold text-gray-900 dark:text-white">
        Supported Algorithms
      </h3>
    </div>
    <div className="flex flex-wrap gap-2" role="list" aria-label="List of supported skeletonization algorithms">
      {algorithms.map((algo) => (
        <span
          key={algo}
          role="listitem"
          className="rounded-lg bg-gradient-to-r from-cyan-600 to-blue-600 px-3 py-1.5 text-xs font-medium text-white shadow-sm"
        >
          {algo}
        </span>
      ))}
    </div>
  </section>
);
