import { headers } from "next/headers";
import { redirect } from "next/navigation";

import { auth } from "@/auth";
import { getImagesByUserId } from "@/repositories/image";

import { ImagesHeader } from "./images-header";
import ImageGallery from "./image-gallery";

const ImagesPage = async () => {
  const session = await auth.api.getSession({ headers: await headers() });

  if (!session?.user) {
    redirect("/");
  }

  const userImages = await getImagesByUserId(session.user.id);

  return (
    <div className="mx-auto max-w-6xl space-y-8 px-4 sm:px-6 lg:px-8">
      <ImagesHeader />

      <ImageGallery initialImages={userImages} />
    </div>
  );
};

export default ImagesPage;
