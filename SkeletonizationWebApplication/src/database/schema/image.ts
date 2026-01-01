import { type AnyPgColumn, bigint, integer, pgTable, text, timestamp, uuid } from "drizzle-orm/pg-core";

import { user } from "./auth";

const IMAGE_STATUS_ENUM = ["uploaded", "skeletonized", "archived", "derived"] as const;

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
  createdAt: timestamp("created_at").defaultNow().notNull()
});
