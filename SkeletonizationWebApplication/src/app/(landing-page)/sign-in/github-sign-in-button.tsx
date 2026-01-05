"use client";

import { authClient } from "@/auth-client";

export const GithubSignInButton = () => {
  const handleGithubSignIn = async () => {
    await authClient.signIn.social({
      provider: "github",
      callbackURL: "/"
    });
  };

  return (
    <button
      onClick={handleGithubSignIn}
      className="group relative flex w-full items-center justify-center gap-3 overflow-hidden rounded-xl border-2 border-gray-200 bg-white px-6 py-4 text-base font-semibold text-gray-700 shadow-lg transition-all hover:border-gray-500 hover:shadow-xl focus:ring-4 focus:ring-gray-500/50 focus:outline-none xl:gap-2 xl:px-5 xl:py-3 xl:text-sm xl:shadow-md 2xl:gap-3 2xl:px-6 2xl:py-4 2xl:text-base 2xl:shadow-lg dark:border-gray-700 dark:bg-gray-900 dark:text-gray-200 dark:hover:border-gray-400"
      aria-label="Sign in with GitHub"
      type="button"
    >
      <svg
        className="h-6 w-6 transition-transform group-hover:scale-110 xl:h-5 xl:w-5 2xl:h-6 2xl:w-6"
        viewBox="0 0 24 24"
        aria-hidden="true"
        fill="currentColor"
      >
        <path d="M12 .5C5.73.5.75 5.66.75 12.07c0 5.13 3.16 9.48 7.55 11.02.55.1.75-.24.75-.54 0-.27-.01-.98-.02-1.93-3.07.69-3.72-1.51-3.72-1.51-.5-1.3-1.22-1.65-1.22-1.65-1-.7.08-.69.08-.69 1.1.08 1.68 1.16 1.68 1.16.98 1.72 2.57 1.22 3.2.93.1-.73.38-1.22.69-1.5-2.45-.29-5.02-1.25-5.02-5.56 0-1.23.42-2.23 1.12-3.02-.11-.28-.49-1.43.11-2.98 0 0 .92-.3 3.01 1.15.87-.25 1.8-.37 2.73-.38.93.01 1.86.13 2.73.38 2.1-1.45 3.01-1.15 3.01-1.15.6 1.55.22 2.7.11 2.98.7.79 1.12 1.79 1.12 3.02 0 4.32-2.58 5.26-5.04 5.55.39.35.74 1.04.74 2.1 0 1.51-.01 2.73-.01 3.1 0 .3.2.65.76.54 4.39-1.54 7.55-5.89 7.55-11.02C23.25 5.66 18.27.5 12 .5z" />
      </svg>
      <span>Continue with GitHub</span>
    </button>
  );
};
