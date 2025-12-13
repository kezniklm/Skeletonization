export const algorithms = [
  "Zhang-Suen",
  "Guo-Hall",
  "K3M",
  "KMM",
  "Choi-Lam-Siu",
  "Han-La-Rhee",
  "Kwon-Kang",
  "Petrosino-Salvi",
  "Tarabek"
] as const;

export type Algorithm = (typeof algorithms)[number];
