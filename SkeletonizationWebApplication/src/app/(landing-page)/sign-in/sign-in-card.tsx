import { Network } from "lucide-react";

import { GithubSignInButton } from "./github-sign-in-button";
import { GoogleSignInButton } from "./google-sign-in-button";
import { signInFeatures } from "./sign-in-features";

export const SignInCard = () => (
  <section className="w-full max-w-md lg:w-auto xl:max-w-sm 2xl:max-w-md" aria-labelledby="sign-in-heading">
    <div className="relative overflow-hidden rounded-3xl bg-white/90 shadow-2xl backdrop-blur-xl xl:rounded-2xl xl:shadow-xl 2xl:rounded-3xl 2xl:shadow-2xl dark:bg-gray-800/90">
      <div
        className="absolute inset-0 rounded-3xl bg-linear-to-r from-blue-600 via-purple-600 to-pink-600 p-0.5 xl:rounded-2xl 2xl:rounded-3xl"
        aria-hidden="true"
      >
        <div className="h-full w-full rounded-3xl bg-white xl:rounded-2xl 2xl:rounded-3xl dark:bg-gray-800" />
      </div>

      <div className="relative space-y-8 p-8 sm:p-10 xl:space-y-6 xl:p-7 2xl:space-y-8 2xl:p-10">
        <header className="text-center">
          <div
            className="mb-4 inline-flex items-center justify-center rounded-full bg-linear-to-r from-cyan-600 to-blue-600 p-3 xl:p-2.5 2xl:p-3"
            aria-hidden="true"
          >
            <Network className="h-6 w-6 text-white xl:h-5 xl:w-5 2xl:h-6 2xl:w-6" />
          </div>
          <h2
            id="sign-in-heading"
            className="mb-2 text-2xl font-bold text-gray-900 xl:text-xl 2xl:text-2xl dark:text-white"
          >
            Access Your Lab
          </h2>
          <p className="text-sm text-gray-600 xl:text-xs 2xl:text-sm dark:text-gray-400">
            Sign in to start processing images
          </p>
        </header>

        <div className="space-y-4 xl:space-y-3 2xl:space-y-4">
          <GoogleSignInButton />
          <GithubSignInButton />

          <div className="relative" role="separator" aria-label="Authentication method separator">
            <div className="absolute inset-0 flex items-center">
              <div className="w-full border-t border-gray-200 dark:border-gray-700" />
            </div>
            <div className="relative flex justify-center text-xs">
              <span className="bg-white px-2 text-gray-500 dark:bg-gray-800 dark:text-gray-400">
                Secure OAuth Authentication
              </span>
            </div>
          </div>
        </div>

        <div
          className="space-y-3 rounded-xl bg-linear-to-r from-cyan-50 to-blue-50 p-4 xl:space-y-2 xl:p-3.5 2xl:space-y-3 2xl:p-4 dark:from-gray-900 dark:to-gray-800"
          role="list"
          aria-label="Key features"
        >
          {signInFeatures.map(({ icon: Icon, label }) => (
            <div
              key={label}
              role="listitem"
              className="flex items-center space-x-2 text-sm text-gray-700 xl:space-x-1.5 xl:text-xs 2xl:space-x-2 2xl:text-sm dark:text-gray-300"
            >
              <Icon
                className="h-4 w-4 text-cyan-600 xl:h-3.5 xl:w-3.5 2xl:h-4 2xl:w-4 dark:text-cyan-400"
                aria-hidden="true"
              />
              <span>{label}</span>
            </div>
          ))}
        </div>

        <footer className="text-center text-xs text-gray-500 dark:text-gray-400">
          By signing in, you agree to our{" "}
          <a href="/terms" className="font-medium text-blue-600 hover:underline dark:text-blue-400">
            Terms of Service
          </a>{" "}
          and{" "}
          <a href="/privacy" className="font-medium text-blue-600 hover:underline dark:text-blue-400">
            Privacy Policy
          </a>
        </footer>
      </div>
    </div>
  </section>
);
