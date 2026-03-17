/**
 * @file step-indicator.tsx
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Multi-step progress indicator component.
 * @description Renders workflow steps with completion states and a visual progress line.
 * @version 1.0
 * @date 2026-03-16
 */

import { Check } from "lucide-react";

import { cn } from "@/lib/utils";

type Step = {
  id: number;
  title: string;
  description: string;
};

type StepIndicatorProps = {
  steps: Step[];
  currentStep: number;
};

type StepStatus = "completed" | "current" | "upcoming";

/**
 * @brief Resolves display status for a workflow step.
 */
const getStepStatus = (stepId: number, currentStep: number): StepStatus => {
  if (stepId < currentStep) {
    return "completed";
  }

  if (stepId === currentStep) {
    return "current";
  }

  return "upcoming";
};

/**
 * @brief Calculates progress bar width from current step position.
 */
const getProgressWidth = (stepsLength: number, currentStep: number) => {
  if (stepsLength <= 1) {
    return 0;
  }

  const clampedIndex = Math.min(Math.max(currentStep - 1, 0), stepsLength - 1);

  return (clampedIndex / (stepsLength - 1)) * 100;
};

/**
 * @brief Displays a single step marker and metadata.
 */
const StepItem = ({ step, status }: { step: Step; status: StepStatus }) => {
  const isCompleted = status === "completed";
  const isCurrent = status === "current";
  const isUpcoming = status === "upcoming";

  return (
    <div className="flex flex-1 flex-col items-center">
      <div
        className={cn(
          "flex h-10 w-10 items-center justify-center rounded-full border-2 bg-white transition-all duration-300 dark:bg-gray-900",
          isCompleted && "border-cyan-500 bg-cyan-500 dark:border-cyan-400 dark:bg-cyan-400",
          isCurrent && "border-cyan-500 ring-4 ring-cyan-500/20 dark:border-cyan-400 dark:ring-cyan-400/20",
          isUpcoming && "border-gray-300 dark:border-gray-700"
        )}
      >
        {isCompleted ? (
          <Check className="h-5 w-5 text-white" />
        ) : (
          <span
            className={cn(
              "text-sm font-semibold",
              isCurrent && "text-cyan-600 dark:text-cyan-400",
              isUpcoming && "text-gray-400 dark:text-gray-600"
            )}
          >
            {step.id}
          </span>
        )}
      </div>

      <div className="mt-3 text-center">
        <p
          className={cn(
            "text-sm font-medium",
            (isCompleted || isCurrent) && "text-gray-900 dark:text-white",
            isUpcoming && "text-gray-500 dark:text-gray-500"
          )}
        >
          {step.title}
        </p>
        <p
          className={cn(
            "mt-0.5 text-xs",
            (isCompleted || isCurrent) && "text-gray-600 dark:text-gray-400",
            isUpcoming && "text-gray-400 dark:text-gray-600"
          )}
        >
          {step.description}
        </p>
      </div>
    </div>
  );
};

/**
 * @brief Displays a horizontal progress indicator for sequential steps.
 */
export const StepIndicator = ({ steps, currentStep }: StepIndicatorProps) => {
  const progressWidth = getProgressWidth(steps.length, currentStep);

  return (
    <div className="mb-8 w-full lg:mb-4 2xl:mb-8">
      <div className="relative">
        <div className="absolute top-5 right-0 left-0 h-0.5 bg-gray-200 dark:bg-gray-700" />

        <div
          className="absolute top-5 left-0 h-0.5 bg-linear-to-r from-cyan-500 to-blue-500 transition-all duration-500 dark:from-cyan-400 dark:to-blue-400"
          style={{ width: `${progressWidth}%` }}
        />

        <div className="relative flex justify-between">
          {steps.map((step) => (
            <StepItem key={step.id} step={step} status={getStepStatus(step.id, currentStep)} />
          ))}
        </div>
      </div>
    </div>
  );
};
