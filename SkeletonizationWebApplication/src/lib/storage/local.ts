/**
 * @file local.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Local filesystem storage backend implementation.
 * @description Implements object storage interface using project `public` directory for local development and deployment.
 * @version 1.0
 * @date 2026-03-16
 */

import { mkdir, readFile, unlink, writeFile } from "fs/promises";
import { dirname } from "path";

import type { StorageConfig } from "./config";

import type { ObjectStorage, PutOptions, StorageObject } from "./index";
import { buildLocalFullPath, buildLocalPublicUrl } from "./index";

/** @brief Creates local filesystem-backed object storage implementation. */
export const createLocalStorage = (config: Extract<StorageConfig, { backend: "local" }>): ObjectStorage => ({
  config,

  putObject: async (key: string, body: Buffer, _options?: PutOptions): Promise<StorageObject> => {
    const fullPath = buildLocalFullPath(key);
    await mkdir(dirname(fullPath), { recursive: true });
    await writeFile(fullPath, body);
    return { key, url: buildLocalPublicUrl(key) };
  },

  getObject: async (key: string) => {
    const fullPath = buildLocalFullPath(key);
    const body = await readFile(fullPath);
    return { body };
  },

  deleteObject: async (key: string) => {
    const fullPath = buildLocalFullPath(key);
    try {
      await unlink(fullPath);
    } catch {
      // Ignore missing files
    }
  },

  getPublicUrl: (key: string) => buildLocalPublicUrl(key),

  isLocalPathKey: (key: string) =>
    ((normalized) => normalized.startsWith("uploads/") || normalized.startsWith("/uploads/"))(key.replace(/\\/g, "/"))
});
