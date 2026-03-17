/**
 * @file trigger.config.ts
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Defines Trigger.dev runtime configuration.
 * @description Configures Trigger project id, runtime, retries, machine class, and task discovery directories.
 * @version 1.0
 * @date 2026-03-16
 */

import { defineConfig } from "@trigger.dev/sdk/v3";

export default defineConfig({
  project: "proj_vixrwujgsmedcucwgfjf",
  runtime: "node",
  logLevel: "log",
  machine: "micro",
  maxDuration: 3600,
  retries: {
    enabledInDev: true,
    default: {
      maxAttempts: 3,
      minTimeoutInMs: 1000,
      maxTimeoutInMs: 10000,
      factor: 2,
      randomize: true
    }
  },
  dirs: ["./src/trigger"]
});
