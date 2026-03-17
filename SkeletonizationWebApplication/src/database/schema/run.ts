/**
 * @file run.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Defines skeletonization run table schema.
 * @description Declares run metadata including owner, name, serialized params, status, and lifecycle timestamps.
 * @version 1.0
 * @date 2026-03-16
 */

import { jsonb, pgTable, text, timestamp, uuid } from "drizzle-orm/pg-core";

import { user } from "./auth";

const RUN_STATUS_ENUM = ["pending", "running", "failed", "completed", "cancelled"] as const;

/** @brief Database table schema for skeletonization runs. */
export const run = pgTable("run", {
  id: uuid("id").primaryKey().defaultRandom(),
  userId: text("user_id")
    .notNull()
    .references(() => user.id, { onDelete: "cascade" }),
  name: text("name").notNull(),
  params: jsonb("params"),
  status: text("status", { enum: RUN_STATUS_ENUM }).default("pending").notNull(),
  createdAt: timestamp("created_at", { withTimezone: true }).defaultNow().notNull(),
  startedAt: timestamp("started_at", { withTimezone: true }),
  completedAt: timestamp("completed_at", { withTimezone: true })
});
