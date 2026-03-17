/**
 * @file not-found.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Implements custom 404 page.
 * @description Shows branded not-found message with navigation action back to landing page.
 * @version 1.0
 * @date 2026-03-16
 */

import Link from "next/link";
import { Compass, ArrowLeft } from "lucide-react";

/**
 * @brief Renders 404 not-found screen.
 * @returns Not-found page JSX.
 */
const NotFound = () => (
  <div className="flex min-h-[78vh] items-center justify-center px-4 sm:px-6">
    <div className="w-full max-w-lg">
      <div className="relative overflow-hidden rounded-3xl bg-white/90 shadow-2xl backdrop-blur-xl dark:bg-gray-900/95">
        <div
          className="absolute inset-0 rounded-3xl bg-linear-to-r from-blue-600 via-purple-600 to-pink-600 p-0.5"
          aria-hidden="true"
        >
          <div className="h-full w-full rounded-3xl bg-white dark:bg-gray-900" />
        </div>

        <div className="relative space-y-6 p-8 sm:p-10">
          <header className="space-y-3 text-center">
            <div
              className="mx-auto inline-flex items-center justify-center rounded-full bg-linear-to-r from-cyan-600 to-blue-600 p-3"
              aria-hidden="true"
            >
              <Compass className="h-6 w-6 text-white" />
            </div>

            <p className="text-xs font-semibold tracking-[0.2em] text-cyan-600 uppercase dark:text-cyan-400">
              Error 404
            </p>

            <h1 className="bg-linear-to-r from-cyan-600 to-blue-600 bg-clip-text text-3xl leading-relaxed font-bold text-transparent sm:text-4xl">
              Page not found
            </h1>

            <p className="text-sm text-gray-600 dark:text-gray-400">
              The page you&apos;re looking for doesn&apos;t exist, was moved, or never existed in the first place.
            </p>
          </header>

          <div className="flex flex-col items-center gap-3 pt-2 sm:flex-row sm:justify-center">
            <Link
              href="/"
              className="group inline-flex items-center justify-center gap-2 rounded-xl bg-linear-to-r from-cyan-600 to-blue-600 px-6 py-2.5 text-sm font-semibold text-white shadow-lg transition-all hover:from-cyan-700 hover:to-blue-700 hover:shadow-xl focus:ring-4 focus:ring-blue-500/40 focus:outline-none"
            >
              <ArrowLeft className="h-4 w-4 transition-transform group-hover:-translate-x-0.5" />
              <span>Go back home</span>
            </Link>
          </div>

          <p className="pt-2 text-center text-xs text-gray-500 dark:text-gray-500">
            If you believe this is an error, please double-check the URL or return to the dashboard.
          </p>
        </div>
      </div>
    </div>
  </div>
);

export default NotFound;
