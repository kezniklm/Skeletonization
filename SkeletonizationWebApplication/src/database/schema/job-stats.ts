import { integer, pgTable, text, timestamp, uuid } from "drizzle-orm/pg-core";

import { job } from "./job";

export const workerTypeEnum = ["cpu", "mt", "gpu"] as const;

export const jobStats = pgTable("job_stats", {
  jobId: uuid("job_id")
    .primaryKey()
    .references(() => job.id, { onDelete: "cascade" }),
  workerId: text("worker_id").notNull(),
  workerType: text("worker_type", { enum: workerTypeEnum }).notNull(),
  lastError: text("last_error"),
  logs: text("logs"),
  processingTimeMs: integer("processing_time_ms"),
  startedAt: timestamp("started_at", { withTimezone: true }),
  finishedAt: timestamp("finished_at", { withTimezone: true }),
  createdAt: timestamp("created_at", { withTimezone: true }).defaultNow().notNull()
});
