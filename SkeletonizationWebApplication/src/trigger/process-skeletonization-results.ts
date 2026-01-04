import { logger, schedules, wait } from "@trigger.dev/sdk/v3";

import { processBatch } from "@/lib/job-result-processor";

type ProcessingSummary = {
  processed: number;
  errors: number;
  durationMs: number;
  iterations: number;
};

const CRON_EVERY_MINUTE = "*/1 * * * *";

const TIME_BUDGET_MS = 60_000;
const EMPTY_QUEUE_POLL_SECONDS = 5;

const MAX_BATCH_SIZE = 200;
const MAX_BATCH_TIME_MS = 25_000;

const remainingMs = (startedAt: number, budgetMs: number): number => Math.max(0, budgetMs - (Date.now() - startedAt));

const processSkeletonizationResultsWithinBudget = async (
  startedAt: number,
  budgetMs: number
): Promise<Omit<ProcessingSummary, "durationMs">> => {
  const counters = {
    processed: 0,
    errors: 0,
    iterations: 0
  };

  while (remainingMs(startedAt, budgetMs) > 0) {
    counters.iterations++;

    const batchTimeBudgetMs = Math.min(MAX_BATCH_TIME_MS, remainingMs(startedAt, budgetMs));
    const batch = await processBatch(MAX_BATCH_SIZE, batchTimeBudgetMs);

    counters.processed += batch.processed;
    counters.errors += batch.errors;

    const noWorkFound = batch.processed === 0;
    const likelyQueueDrained = batch.processed < MAX_BATCH_SIZE && !batch.stoppedEarly;

    if (noWorkFound || likelyQueueDrained) {
      if (remainingMs(startedAt, budgetMs) > 0) {
        await wait.for({ seconds: EMPTY_QUEUE_POLL_SECONDS });
        continue;
      }

      break;
    }
  }

  return counters;
};

export const processSkeletonizationResults = schedules.task({
  id: "process-skeletonization-results",
  cron: CRON_EVERY_MINUTE,
  maxDuration: 65,
  run: async () => {
    const startedAt = Date.now();
    const result = await processSkeletonizationResultsWithinBudget(startedAt, TIME_BUDGET_MS);

    const summary: ProcessingSummary = {
      ...result,
      durationMs: Date.now() - startedAt
    };

    logger.log("Processed skeletonization results", summary);
    return summary;
  }
});
