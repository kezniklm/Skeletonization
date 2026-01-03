import { join } from "path";

import { getStorageConfig, type StorageConfig } from "./config";
import { createLocalStorage } from "./local";
import { createS3Storage } from "./s3";

export type PutOptions = {
  contentType?: string;
};

export type StorageObject = {
  key: string;
  url: string;
};

export type ObjectStorage = {
  config: StorageConfig;

  putObject: (key: string, body: Buffer, options?: PutOptions) => Promise<StorageObject>;
  getObject: (key: string) => Promise<{ body: Buffer; contentType?: string }>;
  deleteObject: (key: string) => Promise<void>;

  getPublicUrl: (key: string) => string;
  isLocalPathKey: (key: string) => boolean;
};

export const getStorage = (): ObjectStorage => {
  const config = getStorageConfig();
  if (config.backend === "local") return createLocalStorage(config);
  return createS3Storage(config);
};

export const buildLocalPublicUrl = (key: string) => {
  const normalized = key.replace(/\\/g, "/");
  return normalized.startsWith("/") ? normalized : `/${normalized}`;
};

export const buildLocalFullPath = (publicKey: string) => join(process.cwd(), "public", publicKey);
