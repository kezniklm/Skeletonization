import { type FileOutputFormat } from "@/database/zod";

export type SortOption = "date-desc" | "date-asc" | "name-asc" | "name-desc";
export type FilterType = "all" | "uploaded" | "skeletonized" | "archived" | "derived";
export type SizeFilter = "small" | "medium" | "large" | "xlarge";
export type ImageFormat = Lowercase<FileOutputFormat>;
