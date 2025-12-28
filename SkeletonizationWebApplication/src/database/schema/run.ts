import { jsonb, pgTable, text, timestamp, uuid } from "drizzle-orm/pg-core";

import { user } from "./auth";

const runStatusEnum = ["pending", "running", "failed", "completed", "cancelled"] as const;

export type RunStatus = (typeof runStatusEnum)[number];

export const run = pgTable("run", {
  id: uuid("id").primaryKey().defaultRandom(),
  userId: text("user_id")
    .notNull()
    .references(() => user.id, { onDelete: "cascade" }),
  name: text("name").notNull(),
  params: jsonb("params"),
  status: text("status", { enum: runStatusEnum }).default("pending").notNull(),
  createdAt: timestamp("created_at").defaultNow().notNull(),
  startedAt: timestamp("started_at"),
  completedAt: timestamp("completed_at")
});
