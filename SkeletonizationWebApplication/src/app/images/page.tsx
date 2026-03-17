/**
 * @file page.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Implements server-rendered image library page.
 * @description Validates session access, loads user images, and renders gallery workspace.
 * @version 1.0
 * @date 2026-03-16
 */

import { headers } from "next/headers";
import { redirect } from "next/navigation";

import { auth } from "@/auth";
import { getImagesByUserId } from "@/repositories/image";

import { ImagesHeader } from "./images-header";
import ImageGallery from "./image-gallery";

/**
 * @brief Renders authenticated image library route.
 * @returns Image page layout JSX.
 */
const ImagesPage = async () => {
  const session = await auth.api.getSession({ headers: await headers() });

  if (!session?.user) {
    redirect("/");
  }

  const userImages = await getImagesByUserId(session.user.id);

  return (
    <div className="mx-auto max-w-4xl space-y-8 px-4 sm:px-6 lg:px-0 xl:space-y-6 2xl:max-w-5xl 2xl:space-y-8">
      <ImagesHeader />

      <ImageGallery initialImages={userImages} />
    </div>
  );
};

export default ImagesPage;
