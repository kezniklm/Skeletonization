/**
 * @file config.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Resolves object storage configuration from environment.
 * @description Defines local and S3 storage config shapes and helper to load validated storage backend settings.
 * @version 1.0
 * @date 2026-03-16
 */

/** @brief Supported object storage backend values. */
export type StorageBackend = "local" | "s3";

/** @brief Discriminated union for supported storage backend configuration. */
export type StorageConfig =
  | {
      backend: "local";
      publicUploadsDir: string;
    }
  | {
      backend: "s3";
      bucket: string;
      region: string;
      endpoint: string;
      accessKeyId: string;
      secretAccessKey: string;
      forcePathStyle: boolean;
      publicBaseUrl?: string;
    };

const requireEnv = (key: string): string => {
  const value = process.env[key];
  if (!value) {
    throw new Error(`Missing required env var: ${key}`);
  }
  return value;
};

const parseBool = (value: string | undefined, defaultValue: boolean) => {
  if (value === undefined) return defaultValue;
  const normalized = value.trim().toLowerCase();
  if (["1", "true", "yes", "y", "on"].includes(normalized)) return true;
  if (["0", "false", "no", "n", "off"].includes(normalized)) return false;
  return defaultValue;
};

/** @brief Loads and validates storage configuration from environment variables. */
export const getStorageConfig = (): StorageConfig => {
  const backend = (process.env.STORAGE_BACKEND ?? "local").toLowerCase() as StorageBackend;

  if (backend === "local") {
    return {
      backend: "local",
      publicUploadsDir: process.env.PUBLIC_UPLOADS_DIR ?? "uploads"
    };
  }

  if (backend !== "s3") {
    throw new Error(`Invalid STORAGE_BACKEND: ${process.env.STORAGE_BACKEND}`);
  }

  return {
    backend: "s3",
    bucket: requireEnv("S3_BUCKET"),
    region: process.env.S3_REGION ?? "auto",
    endpoint: requireEnv("S3_ENDPOINT"),
    accessKeyId: requireEnv("S3_ACCESS_KEY_ID"),
    secretAccessKey: requireEnv("S3_SECRET_ACCESS_KEY"),
    forcePathStyle: parseBool(process.env.S3_FORCE_PATH_STYLE, true),
    publicBaseUrl: process.env.S3_PUBLIC_BASE_URL
  };
};
