import { GitBranch } from "lucide-react";

export const Branding = () => (
  <header className="mb-8 text-center lg:text-left">
    <div className="mb-6 inline-flex items-center space-x-3">
      <div
        className="flex h-12 w-12 items-center justify-center rounded-xl bg-gradient-to-br from-cyan-500 to-blue-600 shadow-lg"
        aria-hidden="true"
      >
        <GitBranch className="h-7 w-7 text-white" />
      </div>
      <h1 className="bg-gradient-to-r from-cyan-600 to-blue-600 bg-clip-text text-4xl font-bold text-transparent dark:from-cyan-400 dark:to-blue-400">
        Skeletonizer
      </h1>
    </div>

    <h2 className="mb-4 text-4xl leading-tight font-bold text-gray-900 sm:text-5xl dark:text-white">
      Extract the{" "}
      <span className="bg-gradient-to-r from-cyan-600 to-blue-600 bg-clip-text text-transparent">
        Structural Essence
      </span>{" "}
      of Your Images
    </h2>
    <p className="text-lg text-gray-600 dark:text-gray-300">
      Reduce complex shapes to their fundamental skeletal structure using advanced morphological thinning and medial
      axis algorithms.
    </p>
  </header>
);
