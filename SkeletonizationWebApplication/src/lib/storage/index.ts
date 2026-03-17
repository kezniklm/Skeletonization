/**
 * @file index.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Object storage abstraction entrypoint.
 * @description Defines storage interfaces and selects local or S3 implementation based on resolved config.
 * @version 1.0
 * @date 2026-03-16
 */

import { join } from "path";

import { getStorageConfig, type StorageConfig } from "./config";
import { createLocalStorage } from "./local";
import { createS3Storage } from "./s3";

/**
 * @brief Optional metadata for object uploads.
 */
export type PutOptions = {
  contentType?: string;
};

/**
 * @brief Represents stored object identity and public URL.
 */
export type StorageObject = {
  key: string;
  url: string;
};

/**
 * @brief Interface implemented by object storage backends.
 */
export type ObjectStorage = {
  config: StorageConfig;

  putObject: (key: string, body: Buffer, options?: PutOptions) => Promise<StorageObject>;
  getObject: (key: string) => Promise<{ body: Buffer; contentType?: string }>;
  deleteObject: (key: string) => Promise<void>;

  getPublicUrl: (key: string) => string;
  isLocalPathKey: (key: string) => boolean;
};

/** @brief Returns configured storage implementation instance. */
export const getStorage = (): ObjectStorage => {
  const config = getStorageConfig();
  if (config.backend === "local") return createLocalStorage(config);
  return createS3Storage(config);
};

/** @brief Builds public URL path for locally stored object key. */
export const buildLocalPublicUrl = (key: string) => {
  const normalized = key.replace(/\\/g, "/");
  return normalized.startsWith("/") ? normalized : `/${normalized}`;
};

/** @brief Builds absolute filesystem path for local public object key. */
export const buildLocalFullPath = (publicKey: string) => join(process.cwd(), "public", publicKey);
