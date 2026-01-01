import { integer, jsonb, pgTable, text, timestamp, uuid } from "drizzle-orm/pg-core";

import { ALGORITHMS } from "@/algorithms";

import { image } from "./image";
import { run } from "./run";

const jobStatusEnum = ["queued", "processing", "completed", "failed"] as const;

export type JobStatus = (typeof jobStatusEnum)[number];

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
  status: text("status", { enum: jobStatusEnum }).default("queued").notNull(),
  createdAt: timestamp("created_at").defaultNow().notNull()
});
