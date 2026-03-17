/**
 * @file algorithms.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Defines supported skeletonization algorithms.
 * @description Exports canonical algorithm list and corresponding union type for type-safe references.
 * @version 1.0
 * @date 2026-03-16
 */

/** @brief Ordered list of available skeletonization algorithms. */
export const ALGORITHMS = [
  "Choi-Lam-Siu",
  "Guo-Hall",
  "Han-La-Rhee",
  "K3M",
  "KMM",
  "Kwon-Kang",
  "Lantuejoul",
  "Liu-Zhang",
  "Petrosino-Salvi",
  "Tarabek",
  "Zhang-Suen"
] as const;

/** @brief Union type derived from `ALGORITHMS` list entries. */
export type Algorithm = (typeof ALGORITHMS)[number];
