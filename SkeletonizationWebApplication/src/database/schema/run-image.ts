import { integer, jsonb, pgTable, text, timestamp, uuid } from "drizzle-orm/pg-core";

import { algorithms } from "@/algorithms";

import { image } from "./image";
import { run } from "./run";

export const runImageStatusEnum = ["pending", "processing", "completed", "failed"] as const;

export const runImage = pgTable("run_image", {
  id: uuid("id").primaryKey().defaultRandom(),
  runId: uuid("run_id")
    .notNull()
    .references(() => run.id, { onDelete: "cascade" }),
  imageId: uuid("image_id")
    .notNull()
    .references(() => image.id, { onDelete: "cascade" }),
  algorithm: text("algorithm", { enum: algorithms }).notNull(),
  ordinal: integer("ordinal").notNull(),
  params: jsonb("params"),
  status: text("status", { enum: runImageStatusEnum }).default("pending").notNull(),
  attempt: integer("attempt").default(0).notNull(),
  createdAt: timestamp("created_at").defaultNow().notNull(),
  processedAt: timestamp("processed_at")
});
