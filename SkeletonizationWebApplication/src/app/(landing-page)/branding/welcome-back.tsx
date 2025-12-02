import { ArrowRight, Sparkles } from "lucide-react";

import { NavigationLink } from "@/components/navigation/navigation-link";

type WelcomeBackProps = {
  className?: string;
  userName?: string;
};

export const WelcomeBack = ({ className, userName }: WelcomeBackProps) => (
  <section className={className} aria-labelledby="welcome-heading">
    <div className="relative overflow-hidden rounded-3xl bg-white/90 shadow-2xl backdrop-blur-xl xl:rounded-2xl xl:shadow-xl 2xl:rounded-3xl 2xl:shadow-2xl dark:bg-gray-800/90">
      <div
        className="absolute inset-0 rounded-3xl bg-linear-to-r from-blue-600 via-purple-600 to-pink-600 p-0.5 xl:rounded-2xl 2xl:rounded-3xl"
        aria-hidden="true"
      >
        <div className="h-full w-full rounded-3xl bg-white xl:rounded-2xl 2xl:rounded-3xl dark:bg-gray-800" />
      </div>

      <div className="relative space-y-6 p-8 sm:p-10 xl:space-y-5 xl:p-7 2xl:space-y-6 2xl:p-10">
        <header className="text-center">
          <div
            className="mb-4 inline-flex items-center justify-center rounded-full bg-linear-to-r from-cyan-600 to-blue-600 p-3 xl:p-2.5 2xl:p-3"
            aria-hidden="true"
          >
            <Sparkles className="h-6 w-6 text-white xl:h-5 xl:w-5 2xl:h-6 2xl:w-6" />
          </div>
          <h2
            id="welcome-heading"
            className="mb-2 text-2xl font-bold text-gray-900 xl:text-xl 2xl:text-2xl dark:text-white"
          >
            Welcome Back{userName ? `, ${userName}` : ""}!
          </h2>
          <p className="text-sm text-gray-600 xl:text-xs 2xl:text-sm dark:text-gray-400">
            Ready to continue your work?
          </p>
        </header>

        <NavigationLink
          href="/dashboard"
          className="group relative flex w-full items-center justify-center gap-3 overflow-hidden rounded-xl bg-linear-to-r from-cyan-600 to-blue-600 px-6 py-4 text-base font-semibold text-white shadow-lg transition-all hover:from-cyan-700 hover:to-blue-700 hover:shadow-xl focus:ring-4 focus:ring-blue-500/50 focus:outline-none xl:gap-2 xl:px-5 xl:py-3 xl:text-sm xl:shadow-md 2xl:gap-3 2xl:px-6 2xl:py-4 2xl:text-base 2xl:shadow-lg"
          aria-label="Go to dashboard"
        >
          <span>Go to Lab</span>
          <ArrowRight
            className="h-5 w-5 transition-transform group-hover:translate-x-1 xl:h-4 xl:w-4 2xl:h-5 2xl:w-5"
            aria-hidden="true"
          />
        </NavigationLink>

        <div className="space-y-3 rounded-xl border border-cyan-100 bg-linear-to-br from-cyan-50/80 to-blue-50/80 p-4 backdrop-blur-sm xl:space-y-2 xl:p-3 2xl:space-y-3 2xl:p-4 dark:border-gray-700 dark:from-gray-800/50 dark:to-gray-900/50">
          <p className="text-center text-sm font-medium text-gray-700 xl:text-xs 2xl:text-sm dark:text-gray-300">
            Quick Actions
          </p>
          <div className="space-y-2 xl:space-y-1.5 2xl:space-y-2">
            <NavigationLink
              href="/images"
              className="block w-full rounded-lg border border-cyan-200 bg-white/80 px-4 py-2.5 text-center text-sm font-medium text-gray-700 shadow-sm backdrop-blur-sm transition-all hover:border-cyan-300 hover:bg-white hover:shadow-md xl:px-3.5 xl:py-2 xl:text-xs 2xl:px-4 2xl:py-2.5 2xl:text-sm dark:border-gray-600 dark:bg-gray-700/80 dark:text-gray-200 dark:hover:border-gray-500 dark:hover:bg-gray-700"
            >
              Upload New Image
            </NavigationLink>
            <NavigationLink
              href="/history"
              className="block w-full rounded-lg border border-blue-200 bg-white/80 px-4 py-2.5 text-center text-sm font-medium text-gray-700 shadow-sm backdrop-blur-sm transition-all hover:border-blue-300 hover:bg-white hover:shadow-md xl:px-3.5 xl:py-2 xl:text-xs 2xl:px-4 2xl:py-2.5 2xl:text-sm dark:border-gray-600 dark:bg-gray-700/80 dark:text-gray-200 dark:hover:border-gray-500 dark:hover:bg-gray-700"
            >
              View Processing History
            </NavigationLink>
          </div>
        </div>
      </div>
    </div>
  </section>
);
