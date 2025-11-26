import { bigint, jsonb, pgTable, text, timestamp, uuid } from "drizzle-orm/pg-core";

import { runImage } from "./run-image";

export const outputTypeEnum = [
  "original",
  "thumbnail",
  "preprocessed",
  "skeleton",
  "skeleton_overlay",
  "binary_mask",
  "vector",
  "metrics_json"
] as const;

export const output = pgTable("output", {
  id: uuid("id").primaryKey().defaultRandom(),
  runImageId: uuid("run_image_id")
    .notNull()
    .references(() => runImage.id, { onDelete: "cascade" }),
  type: text("type", { enum: outputTypeEnum }).notNull(),
  storagePath: text("storage_path").notNull(),
  url: text("url").notNull(),
  mime: text("mime").notNull(),
  metrics: jsonb("metrics"),
  sizeBytes: bigint("size_bytes", { mode: "number" }).notNull(),
  createdAt: timestamp("created_at").defaultNow().notNull()
});
