/**
 * @file s3.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief S3-compatible object storage backend implementation.
 * @description Implements object storage interface with S3 uploads/downloads/deletes and signed URL helper.
 * @version 1.0
 * @date 2026-03-16
 */

import { DeleteObjectCommand, GetObjectCommand, PutObjectCommand, S3Client } from "@aws-sdk/client-s3";
import { getSignedUrl } from "@aws-sdk/s3-request-presigner";

import type { StorageConfig } from "./config";

import type { ObjectStorage, PutOptions, StorageObject } from "./index";

const toBuffer = async (body: unknown): Promise<Buffer> => {
  // eslint-disable-next-line @typescript-eslint/no-explicit-any
  const stream: any = body;

  if (!stream) {
    return Buffer.alloc(0);
  }

  if (Buffer.isBuffer(stream)) {
    return stream;
  }

  if (typeof stream.transformToByteArray === "function") {
    const bytes = await stream.transformToByteArray();
    return Buffer.from(bytes);
  }

  return new Promise<Buffer>((resolve, reject) => {
    const chunks: Buffer[] = [];
    stream.on("data", (chunk: Buffer) => chunks.push(Buffer.from(chunk)));
    stream.on("end", () => resolve(Buffer.concat(chunks)));
    stream.on("error", reject);
  });
};

const joinUrl = (base: string, key: string) => {
  const trimmedBase = base.replace(/\/+$/, "");
  const trimmedKey = key.replace(/^\/+/, "");
  return `${trimmedBase}/${trimmedKey}`;
};

/** @brief Creates an S3-backed object storage implementation. */
export const createS3Storage = (config: Extract<StorageConfig, { backend: "s3" }>): ObjectStorage => {
  const client = new S3Client({
    region: config.region,
    endpoint: config.endpoint,
    forcePathStyle: config.forcePathStyle,
    credentials: {
      accessKeyId: config.accessKeyId,
      secretAccessKey: config.secretAccessKey
    }
  });

  const getPublicUrl = (key: string) => {
    if (config.publicBaseUrl) {
      return joinUrl(config.publicBaseUrl, key);
    }
    // Private bucket fallback: serve via app route that will mint a short-lived signed URL.
    return `/api/storage/object?key=${encodeURIComponent(key)}`;
  };

  return {
    config,

    putObject: async (key: string, body: Buffer, options?: PutOptions): Promise<StorageObject> => {
      await client.send(
        new PutObjectCommand({
          Bucket: config.bucket,
          Key: key,
          Body: body,
          ContentType: options?.contentType
        })
      );
      return { key, url: getPublicUrl(key) };
    },

    getObject: async (key: string) => {
      const response = await client.send(
        new GetObjectCommand({
          Bucket: config.bucket,
          Key: key
        })
      );
      const body = await toBuffer(response.Body);
      return { body, contentType: response.ContentType };
    },

    deleteObject: async (key: string) => {
      await client.send(
        new DeleteObjectCommand({
          Bucket: config.bucket,
          Key: key
        })
      );
    },

    getPublicUrl,

    isLocalPathKey: () => false
  };
};

/** @brief Creates short-lived signed GET URL for private S3 object key. */
export const createSignedGetUrl = async (
  config: Extract<StorageConfig, { backend: "s3" }>,
  key: string,
  expiresSeconds = 60
) => {
  const client = new S3Client({
    region: config.region,
    endpoint: config.endpoint,
    forcePathStyle: config.forcePathStyle,
    credentials: {
      accessKeyId: config.accessKeyId,
      secretAccessKey: config.secretAccessKey
    }
  });

  return getSignedUrl(
    client,
    new GetObjectCommand({
      Bucket: config.bucket,
      Key: key
    }),
    { expiresIn: expiresSeconds }
  );
};
