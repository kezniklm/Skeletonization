export const ALGORITHMS = [
  "Choi-Lam-Siu",
  "Guo-Hall",
  "Han-La-Rhee",
  "K3M",
  "KMM",
  "Kwon-Kang",
  "Petrosino-Salvi",
  "Tarabek",
  "Zhang-Suen"
] as const;

export type Algorithm = (typeof ALGORITHMS)[number];
