/**
 * @file image.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Defines image table schema.
 * @description Declares persisted image metadata including ownership, storage references, dimensions, status, and lineage.
 * @version 1.0
 * @date 2026-03-16
 */

import { type AnyPgColumn, bigint, integer, pgTable, text, timestamp, uuid } from "drizzle-orm/pg-core";

import { user } from "./auth";

const IMAGE_STATUS_ENUM = ["uploaded", "skeletonized", "archived", "derived"] as const;

/** @brief Database table schema for image metadata and lifecycle state. */
export const image = pgTable("image", {
  id: uuid("id").primaryKey().defaultRandom(),
  userId: text("user_id")
    .notNull()
    .references(() => user.id, { onDelete: "cascade" }),
  originalFilename: text("original_filename").notNull(),
  storagePath: text("storage_path").notNull(),
  url: text("url").notNull(),
  mime: text("mime").notNull(),
  width: integer("width").notNull(),
  height: integer("height").notNull(),
  sizeBytes: bigint("size_bytes", { mode: "number" }).notNull(),
  checksum: text("checksum").notNull(),
  status: text("status", { enum: IMAGE_STATUS_ENUM }).default("uploaded").notNull(),
  parentImageId: uuid("parent_image_id").references((): AnyPgColumn => image.id, { onDelete: "set null" }),
  generatedByJobId: uuid("generated_by_job_id"),
  createdAt: timestamp("created_at", { withTimezone: true }).defaultNow().notNull()
});
