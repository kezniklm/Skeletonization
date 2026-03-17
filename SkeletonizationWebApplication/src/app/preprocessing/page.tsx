/**
 * @file page.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Renders the preprocessing page entry point.
 * @description Loads session and image context, then routes users to either the workspace or image selection view.
 */

import { headers } from "next/headers";
import { redirect } from "next/navigation";

import { auth } from "@/auth";
import { getImageByIdAndUserId, getImagesCountByUserId } from "@/repositories/image";
import { getUserDefaultOutputFormatPreferences } from "@/repositories/preferences";

import { PreprocessingSelectImage } from "./preprocessing-select-image";
import { PreprocessingWorkspaceWrapper } from "./preprocessing-workspace-wrapper";

type PreprocessingPageProps = {
  searchParams: Promise<{ imageId?: string }>;
};

/**
 * @brief Resolves query and session data to render the preprocessing experience.
 * @description Fetches selected image and user defaults, then conditionally renders workspace or image picker content.
 * @param searchParams Promise containing optional `imageId` query parameter.
 * @returns A server component that renders preprocessing workspace or selection state.
 */
const PreprocessingPage = async ({ searchParams }: PreprocessingPageProps) => {
  const session = await auth.api.getSession({ headers: await headers() });

  if (!session?.user) {
    redirect("/");
  }

  const { imageId } = await searchParams;

  const [selectedImage, userImagesCount, defaultOutputFormat] = await Promise.all([
    getImageByIdAndUserId(imageId, session.user.id),
    getImagesCountByUserId(session.user.id),
    getUserDefaultOutputFormatPreferences(session.user.id)
  ]);

  return selectedImage ? (
    <PreprocessingWorkspaceWrapper selectedImage={selectedImage} defaultOutputFormat={defaultOutputFormat} />
  ) : (
    <PreprocessingSelectImage availableImagesCount={userImagesCount} />
  );
};

/**
 * @brief Exposes the preprocessing page route component.
 */
export default PreprocessingPage;
