export const generateRandomFilename = (originalName: string): string => {
  const nameParts = originalName.split(".");
  const ext = nameParts.pop();
  const baseName = nameParts.join(".");
  const randomString = Math.random().toString(36).substring(2, 10);
  const timestamp = Date.now();
  return `${baseName}_${timestamp}_${randomString}.${ext}`;
};
