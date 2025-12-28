import { createHash } from "crypto";
import { access, copyFile, mkdir, readFile, stat } from "fs/promises";
import { join } from "path";

import { basename, getImageDimensions, getMimeTypeFromFilename } from "./image-utils";

export type OutputFileInfo = {
  storagePath: string;
  url: string;
  mime: string;
  width: number;
  height: number;
  sizeBytes: number;
  checksum: string;
  originalFilename: string;
};

const WORKER_OUTPUT_PATHS = [
  // Debug build
  (outputPath: string) =>
    join(process.cwd(), "..", "out", "build", "x64-debug-windows-cuda", "SkeletonizationWorker", outputPath),
  // Release build
  (outputPath: string) =>
    join(process.cwd(), "..", "out", "build", "x64-release-windows-cuda", "SkeletonizationWorker", outputPath),
  // Source directory (if worker runs from there)
  (outputPath: string) => join(process.cwd(), "..", "SkeletonizationWorker", outputPath),
  // Same directory as web app
  (outputPath: string) => join(process.cwd(), outputPath)
];

const findWorkerOutputFile = async (outputPath: string): Promise<string | null> => {
  for (const getPath of WORKER_OUTPUT_PATHS) {
    const path = getPath(outputPath);
    try {
      await access(path);
      return path;
    } catch {
      // Try next path
    }
  }
  return null;
};

export const copyOutputFile = async (workerOutputPath: string): Promise<OutputFileInfo> => {
  const outputFileName = basename(workerOutputPath);

  if (!outputFileName) {
    throw new Error(`Invalid output path: ${workerOutputPath}`);
  }

  const workerPath = await findWorkerOutputFile(workerOutputPath);

  if (!workerPath) {
    throw new Error(`Output file not found: ${workerOutputPath}`);
  }

  const storagePath = `uploads/outputs/${outputFileName}`;
  const publicOutputPath = join(process.cwd(), "public", storagePath);
  const outputDir = join(process.cwd(), "public", "uploads", "outputs");

  await mkdir(outputDir, { recursive: true });
  await copyFile(workerPath, publicOutputPath);

  console.log(`Copied output file from ${workerPath} to: ${storagePath}`);

  const mime = getMimeTypeFromFilename(outputFileName);
  const outputBuffer = await readFile(publicOutputPath);
  const checksum = createHash("sha256").update(outputBuffer).digest("hex");
  const dimensions = getImageDimensions(outputBuffer, mime);
  const stats = await stat(publicOutputPath);

  return {
    storagePath,
    url: `/${storagePath}`,
    mime,
    width: dimensions.width,
    height: dimensions.height,
    sizeBytes: stats.size,
    checksum,
    originalFilename: outputFileName
  };
};
