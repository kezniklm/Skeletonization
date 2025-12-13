export const capitalizeFirst = (str: string): string => {
  if (!str) return str;
  return str.charAt(0).toUpperCase() + str.slice(1);
};

export const formatStatus = (status: string): string =>
  status
    .split("_")
    .map((word) => capitalizeFirst(word))
    .join(" ");

export const formatAlgorithmName = (algorithm: string): string => algorithm.replace(/[_-]/g, " ");
