import { jsonb, pgTable, text, timestamp, uuid } from "drizzle-orm/pg-core";

import { algorithm } from "./algorithm";
import { user } from "./auth";

export const runStatusEnum = ["pending", "running", "failed", "completed", "cancelled"] as const;

export const run = pgTable("run", {
  id: uuid("id").primaryKey().defaultRandom(),
  userId: text("user_id")
    .notNull()
    .references(() => user.id, { onDelete: "cascade" }),
  name: text("name").notNull(),
  algorithmId: uuid("algorithm_id")
    .notNull()
    .references(() => algorithm.id, { onDelete: "restrict" }),
  params: jsonb("params"),
  status: text("status", { enum: runStatusEnum }).default("pending").notNull(),
  createdAt: timestamp("created_at").defaultNow().notNull(),
  startedAt: timestamp("started_at"),
  completedAt: timestamp("completed_at")
});
