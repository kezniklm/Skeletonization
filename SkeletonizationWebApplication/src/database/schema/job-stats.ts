import { integer, pgTable, text, timestamp, uuid } from "drizzle-orm/pg-core";

import { job } from "./job";

export const jobStats = pgTable("job_stats", {
  jobId: uuid("job_id")
    .primaryKey()
    .references(() => job.id, { onDelete: "cascade" }),
  workerId: text("worker_id"),
  lastError: text("last_error"),
  logs: text("logs"),
  processingTimeMs: integer("processing_time_ms"),
  startedAt: timestamp("started_at"),
  finishedAt: timestamp("finished_at"),
  createdAt: timestamp("created_at").defaultNow().notNull()
});
