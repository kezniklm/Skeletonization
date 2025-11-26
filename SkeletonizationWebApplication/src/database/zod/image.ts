import { createInsertSchema, createSelectSchema } from "drizzle-zod";
import { type z } from "zod";

import { image } from "../schema";

export const insertImageSchema = createInsertSchema(image, {
  originalFilename: (schema) => schema.min(1, "Filename is required"),
  storagePath: (schema) => schema.min(1, "Storage path is required"),
  url: (schema) => schema.url("Invalid URL"),
  mime: (schema) => schema.min(1, "MIME type is required"),
  width: (schema) => schema.positive("Width must be positive"),
  height: (schema) => schema.positive("Height must be positive"),
  sizeBytes: (schema) => schema.positive("Size must be positive"),
  checksum: (schema) => schema.min(1, "Checksum is required")
});

export const selectImageSchema = createSelectSchema(image);

export const updateImageSchema = insertImageSchema.partial().required({ id: true });

export type InsertImage = z.infer<typeof insertImageSchema>;
export type SelectImage = z.infer<typeof selectImageSchema>;
export type UpdateImage = z.infer<typeof updateImageSchema>;
