import { integer, pgTable, text, timestamp, uuid } from "drizzle-orm/pg-core";

import { runImage } from "./run-image";

export const jobStatusEnum = ["queued", "processing", "completed", "failed", "cancelled"] as const;

export const job = pgTable("job", {
  id: uuid("id").primaryKey().defaultRandom(),
  runImageId: uuid("run_image_id")
    .notNull()
    .references(() => runImage.id, { onDelete: "cascade" }),
  workerId: text("worker_id"),
  status: text("status", { enum: jobStatusEnum }).default("queued").notNull(),
  attempts: integer("attempts").default(0).notNull(),
  lastError: text("last_error"),
  logs: text("logs"),
  startedAt: timestamp("started_at"),
  finishedAt: timestamp("finished_at"),
  createdAt: timestamp("created_at").defaultNow().notNull()
});
