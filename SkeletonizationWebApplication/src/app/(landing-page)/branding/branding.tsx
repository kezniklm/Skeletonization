import { GitBranch } from "lucide-react";

export const Branding = () => (
  <header className="mb-8 text-center lg:text-left xl:mb-6 2xl:mb-8">
    <div className="mb-6 inline-flex items-center space-x-3 xl:space-x-2.5 2xl:space-x-3">
      <div
        className="flex h-12 w-12 items-center justify-center rounded-xl bg-linear-to-br from-cyan-500 to-blue-600 shadow-lg xl:h-10 xl:w-10 xl:rounded-lg 2xl:h-12 2xl:w-12 2xl:rounded-xl"
        aria-hidden="true"
      >
        <GitBranch className="h-7 w-7 text-white xl:h-6 xl:w-6 2xl:h-7 2xl:w-7" />
      </div>
      <h1 className="bg-linear-to-r from-cyan-600 to-blue-600 bg-clip-text text-4xl font-bold text-transparent xl:text-3xl 2xl:text-4xl dark:from-cyan-400 dark:to-blue-400">
        Skeletonizer
      </h1>
    </div>

    <h2 className="mb-4 text-4xl leading-tight font-bold text-gray-900 sm:text-5xl xl:text-4xl xl:leading-snug 2xl:text-5xl dark:text-white">
      Extract the{" "}
      <span className="bg-linear-to-r from-cyan-600 to-blue-600 bg-clip-text text-transparent">
        Structural
        <br />
        Essence
      </span>{" "}
      of Your Images
    </h2>
    <p className="text-lg text-gray-600 xl:text-base 2xl:text-lg dark:text-gray-300">
      Reduce complex shapes to their fundamental skeletal structure using advanced morphological thinning and medial
      axis algorithms.
    </p>
  </header>
);
