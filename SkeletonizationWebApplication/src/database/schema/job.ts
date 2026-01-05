import { integer, jsonb, pgTable, text, timestamp, uuid } from "drizzle-orm/pg-core";

import { ALGORITHMS } from "@/algorithms";

import { image } from "./image";
import { run } from "./run";

const JOB_STATUS_ENUM = ["queued", "processing", "completed", "failed"] as const;

export const job = pgTable("job", {
  id: uuid("id").primaryKey().defaultRandom(),
  runId: uuid("run_id")
    .notNull()
    .references(() => run.id, { onDelete: "cascade" }),
  imageId: uuid("image_id")
    .notNull()
    .references(() => image.id, { onDelete: "cascade" }),
  algorithm: text("algorithm", { enum: ALGORITHMS }).notNull(),
  ordinal: integer("ordinal").notNull(),
  params: jsonb("params"),
  status: text("status", { enum: JOB_STATUS_ENUM }).default("queued").notNull(),
  createdAt: timestamp("created_at", { withTimezone: true }).defaultNow().notNull()
});
