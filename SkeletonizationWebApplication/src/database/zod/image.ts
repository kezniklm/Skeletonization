/**
 * @file image.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Defines Zod schemas and types for image records.
 * @description Provides insert/select/update schema derivations and type aliases for image entities.
 * @version 1.0
 * @date 2026-03-16
 */

import { createInsertSchema, createSelectSchema } from "drizzle-zod";
import { type z } from "zod";

import { image } from "../schema";

/** @brief Insert schema for image rows with validation overrides. */
export const insertImageSchema = createInsertSchema(image, {
  originalFilename: (schema) => schema.min(1, "Filename is required"),
  storagePath: (schema) => schema.min(1, "Storage path is required"),
  url: (schema) => schema.url("Invalid URL"),
  mime: (schema) => schema.min(1, "MIME type is required"),
  width: (schema) => schema.positive("Width must be positive"),
  height: (schema) => schema.positive("Height must be positive"),
  sizeBytes: (schema) => schema.positive("Size must be positive"),
  checksum: (schema) => schema.min(1, "Checksum is required")
});

/** @brief Select schema for image rows. */
export const selectImageSchema = createSelectSchema(image);

/** @brief Partial update schema for image rows requiring `id`. */
export const updateImageSchema = insertImageSchema.partial().required({ id: true });

/** @brief Insert-image type inferred from schema. */
export type InsertImage = z.infer<typeof insertImageSchema>;
/** @brief Select-image type inferred from schema. */
export type SelectImage = z.infer<typeof selectImageSchema>;
/** @brief Update-image type inferred from schema. */
export type UpdateImage = z.infer<typeof updateImageSchema>;

/** @brief Union type of available image status values. */
export type ImageStatus = z.infer<typeof selectImageSchema.shape.status>;
