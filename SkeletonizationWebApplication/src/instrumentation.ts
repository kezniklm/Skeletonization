export const register = async () => {
  if (process.env.NEXT_RUNTIME === "nodejs") {
    const { jobResultProcessor } = await import("@/lib/job-result-processor");

    console.log("Initializing job result processor...");
    jobResultProcessor.start();
  }
};
