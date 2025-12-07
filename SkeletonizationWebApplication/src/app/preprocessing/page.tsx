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

export default PreprocessingPage;
