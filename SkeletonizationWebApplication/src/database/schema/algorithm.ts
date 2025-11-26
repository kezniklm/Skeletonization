import { pgTable, text, timestamp, uuid } from "drizzle-orm/pg-core";

export const algorithm = pgTable("algorithm", {
  id: uuid("id").primaryKey().defaultRandom(),
  name: text("name").notNull(),
  description: text("description"),
  createdAt: timestamp("created_at").defaultNow().notNull()
});
