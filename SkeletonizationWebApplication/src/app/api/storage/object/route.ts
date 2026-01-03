import { NextResponse } from "next/server";

import { getStorage } from "@/lib/storage";
import { getStorageConfig } from "@/lib/storage/config";

export const GET = async (request: Request) => {
  const config = getStorageConfig();
  if (config.backend !== "s3") {
    return NextResponse.json({ error: "Storage backend is not S3" }, { status: 400 });
  }

  const url = new URL(request.url);
  const key = url.searchParams.get("key");

  if (!key) {
    return NextResponse.json({ error: "Missing key" }, { status: 400 });
  }

  const isValidPath = key.startsWith("uploads/") || key.startsWith("outputs/");
  if (!isValidPath) {
    return NextResponse.json({ error: "Invalid key" }, { status: 400 });
  }

  try {
    const storage = getStorage();
    const { body, contentType } = await storage.getObject(key);

    return new NextResponse(new Uint8Array(body), {
      status: 200,
      headers: {
        "Content-Type": contentType ?? "application/octet-stream",
        "Content-Length": body.length.toString(),
        "Cache-Control": "public, max-age=31536000, immutable"
      }
    });
  } catch (error) {
    console.error("Failed to fetch object from S3:", error);
    return NextResponse.json({ error: "Object not found" }, { status: 404 });
  }
};
