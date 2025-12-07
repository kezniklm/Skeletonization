import { headers } from "next/headers";
import { redirect } from "next/navigation";

import { auth } from "@/auth";

const SkeletonizationPage = async () => {
  const session = await auth.api.getSession({ headers: await headers() });

  if (!session?.user) {
    redirect("/");
  }

  return (
    <div className="mx-auto max-w-4xl px-4 sm:px-6 lg:px-0 2xl:max-w-5xl">
      <div className="mb-8">
        <h1 className="bg-linear-to-r from-cyan-600 to-blue-600 bg-clip-text text-4xl font-bold text-transparent xl:text-3xl 2xl:text-4xl dark:from-cyan-400 dark:to-blue-400">
          Skeletonization
        </h1>
        <p className="mt-2 text-gray-600 xl:text-sm 2xl:text-base dark:text-gray-400">
          xxxx
        </p>
      </div>
    </div>
  );
};

export default SkeletonizationPage;
