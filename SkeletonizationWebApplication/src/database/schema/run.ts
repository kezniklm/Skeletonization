import { jsonb, pgTable, text, timestamp, uuid } from "drizzle-orm/pg-core";

import { user } from "./auth";

const RUN_STATUS_ENUM = ["pending", "running", "failed", "completed", "cancelled"] as const;

export const run = pgTable("run", {
  id: uuid("id").primaryKey().defaultRandom(),
  userId: text("user_id")
    .notNull()
    .references(() => user.id, { onDelete: "cascade" }),
  name: text("name").notNull(),
  params: jsonb("params"),
  status: text("status", { enum: RUN_STATUS_ENUM }).default("pending").notNull(),
  createdAt: timestamp("created_at").defaultNow().notNull(),
  startedAt: timestamp("started_at"),
  completedAt: timestamp("completed_at")
});
