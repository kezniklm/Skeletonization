"use client";

import { useState } from "react";
import { useRouter } from "next/navigation";
import { Loader2, ArrowRight, ArrowLeft, Check } from "lucide-react";
import { toast } from "sonner";

import { Button } from "@/components/ui/button";
import { Input } from "@/components/ui/input";
import { Label } from "@/components/ui/label";
import { Badge } from "@/components/ui/badge";
import { Tabs, TabsList, TabsTrigger } from "@/components/ui/tabs";
import { Checkbox } from "@/components/ui/checkbox";
import { Tooltip, TooltipContent, TooltipTrigger } from "@/components/ui/tooltip";
import { type SelectImage } from "@/database/zod/image";
import { createSkeletonizationRunsAction } from "@/server-actions/skeletonization";
import { useImageGallery } from "@/hooks/use-image-gallery";
import type { Algorithm } from "@/algorithms";
import { StepIndicator } from "@/components/ui/step-indicator";
import { runConfigurationSchema, type RunConfiguration } from "@/database/zod/run";
import { type FilterType } from "@/components/filters";
import { type FileOutputFormat } from "@/database/zod";

import { ImageFilters } from "../images/image-filters";
import { ImageSort } from "../images/image-sort";
import { ImagePagination } from "../images/image-pagination";

import { SkeletonizationImageGallery } from "./skeletonization-image-gallery";
import { AlgorithmMultiSelector } from "./algorithm-multi-selector";

type SkeletonizationWorkspaceProps = {
  images: SelectImage[];
  defaultOutputFormat: FileOutputFormat;
};

type Step = {
  id: number;
  title: string;
  description: string;
};

const STEPS: Step[] = [
  { id: 1, title: "Configure Run", description: "Name and algorithms" },
  { id: 2, title: "Select Images", description: "Choose images to process" },
  { id: 3, title: "Review & Submit", description: "Confirm and start" }
];

const runStep1Schema = runConfigurationSchema.pick({
  name: true,
  algorithms: true
});

const runStep2Schema = runConfigurationSchema.pick({
  imageIds: true
});

type RunFormErrors = {
  name?: string;
  algorithms?: string;
  imageIds?: string;
};

export const SkeletonizationWorkspace = ({ images, defaultOutputFormat }: SkeletonizationWorkspaceProps) => {
  const router = useRouter();
  const [currentStep, setCurrentStep] = useState(1);
  const [selectedImageIds, setSelectedImageIds] = useState<string[]>([]);
  const [selectedAlgorithms, setSelectedAlgorithms] = useState<Algorithm[]>([]);
  const [runName, setRunName] = useState("");
  const [isSubmitting, setIsSubmitting] = useState(false);
  const [errors, setErrors] = useState<RunFormErrors>({});
  const [preprocessAllImages, setPreprocessAllImages] = useState(true);
  const [preprocessByImageId, setPreprocessByImageId] = useState<Record<string, boolean>>({});

  const setPreprocessAll = (nextValue: boolean) => {
    setPreprocessAllImages(nextValue);
    setPreprocessByImageId({});
  };

  const getShouldPreprocessImage = (imageId: string) => preprocessByImageId[imageId] ?? preprocessAllImages;

  const togglePreprocessForImage = (imageId: string) => {
    setPreprocessByImageId((prev) => {
      const next = { ...prev };
      const current = prev[imageId] ?? preprocessAllImages;
      const toggled = !current;

      if (toggled === preprocessAllImages) {
        delete next[imageId];
      } else {
        next[imageId] = toggled;
      }

      return next;
    });
  };

  const {
    filteredImages,
    paginatedImages,
    filterOptions,
    selectedFilter,
    searchQuery,
    selectedFormats,
    selectedSizes,
    sortBy,
    page,
    totalPages,
    activeFilterCount,
    setSelectedFilter,
    setSearchQuery,
    setSortBy,
    setPage,
    toggleFormat,
    toggleSize,
    clearAllFilters
  } = useImageGallery(images);

  const validateStep1 = () => {
    const result = runStep1Schema.safeParse({
      name: runName.trim(),
      algorithms: selectedAlgorithms
    });

    if (!result.success) {
      const fieldErrors = result.error.flatten().fieldErrors;
      const nameError = fieldErrors.name?.[0];
      const algorithmsError = fieldErrors.algorithms?.[0];

      setErrors((prev) => ({
        ...prev,
        name: nameError,
        algorithms: algorithmsError
      }));

      toast.error(nameError ?? algorithmsError ?? "Please fix validation errors");
      return false;
    }

    setErrors((prev) => ({
      ...prev,
      name: undefined,
      algorithms: undefined
    }));

    return true;
  };

  const validateStep2 = () => {
    const result = runStep2Schema.safeParse({
      imageIds: selectedImageIds
    });

    if (!result.success) {
      const fieldErrors = result.error.flatten().fieldErrors;
      const imageIdsError = fieldErrors.imageIds?.[0];

      setErrors((prev) => ({
        ...prev,
        imageIds: imageIdsError
      }));

      toast.error(imageIdsError ?? "Please select at least one image");
      return false;
    }

    setErrors((prev) => ({
      ...prev,
      imageIds: undefined
    }));

    return true;
  };

  const handleNext = () => {
    if (currentStep === 1 && !validateStep1()) return;
    if (currentStep === 2 && !validateStep2()) return;

    setCurrentStep((prev) => Math.min(prev + 1, STEPS.length));
  };

  const handleBack = () => {
    setCurrentStep((prev) => Math.max(prev - 1, 1));
  };

  const handleSubmit = async () => {
    const selectedOverrides = selectedImageIds.reduce<Record<string, boolean>>((acc, imageId) => {
      const override = preprocessByImageId[imageId];
      if (typeof override === "boolean") acc[imageId] = override;
      return acc;
    }, {});

    const runConfig: RunConfiguration = {
      name: runName.trim(),
      algorithms: selectedAlgorithms,
      imageIds: selectedImageIds
    };

    const result = runConfigurationSchema.safeParse(runConfig);

    if (!result.success) {
      const fieldErrors = result.error.flatten().fieldErrors;

      setErrors({
        name: fieldErrors.name?.[0],
        algorithms: fieldErrors.algorithms?.[0],
        imageIds: fieldErrors.imageIds?.[0]
      });

      if (fieldErrors.name || fieldErrors.algorithms) {
        setCurrentStep(1);
      } else if (fieldErrors.imageIds) {
        setCurrentStep(2);
      }

      toast.error("Please fix validation issues before submitting");
      return;
    }

    setIsSubmitting(true);
    try {
      await createSkeletonizationRunsAction({
        ...runConfig,
        params: {
          preprocessAllImages,
          preprocessByImageId: selectedOverrides,
          outputFormat: defaultOutputFormat
        }
      });

      const algorithmCount = selectedAlgorithms.length;
      const imageCount = selectedImageIds.length;

      toast.success(
        `Started run with ${algorithmCount} algorithm${algorithmCount > 1 ? "s" : ""} and ${imageCount} image${imageCount > 1 ? "s" : ""}!`
      );

      router.push("/lab");
    } catch (error) {
      console.error("Error creating run:", error);
      toast.error(error instanceof Error ? error.message : "Failed to create run");
    } finally {
      setIsSubmitting(false);
    }
  };

  const canProceed = () => {
    switch (currentStep) {
      case 1:
        return runStep1Schema.safeParse({
          name: runName.trim(),
          algorithms: selectedAlgorithms
        }).success;
      case 2:
        return runStep2Schema.safeParse({
          imageIds: selectedImageIds
        }).success;
      case 3:
        return true;
      default:
        return false;
    }
  };

  return (
    <div className="space-y-8 lg:space-y-4 2xl:space-y-8">
      <StepIndicator steps={STEPS} currentStep={currentStep} />

      <div className="mb-8 overflow-hidden rounded-xl border border-gray-200 bg-white shadow-sm backdrop-blur-sm lg:mb-4 xl:rounded-lg 2xl:mb-8 2xl:rounded-xl dark:border-gray-800 dark:bg-gray-900/95">
        {currentStep === 1 && (
          <>
            <div className="border-b border-gray-200 bg-linear-to-r from-cyan-50/50 to-blue-50/50 px-6 py-4 xl:px-5 xl:py-3 2xl:px-6 2xl:py-4 dark:border-gray-800 dark:from-cyan-950/20 dark:to-blue-950/20">
              <h2 className="text-lg font-semibold text-gray-900 xl:text-base 2xl:text-lg dark:text-white">
                Configure Run
              </h2>
              <p className="mt-1 text-sm text-gray-600 xl:text-xs 2xl:text-sm dark:text-gray-400">
                Name your run and select which algorithms to apply
              </p>
            </div>
            <div className="space-y-6 px-6 py-6 lg:space-y-1 lg:px-2 lg:py-1 xl:space-y-6 xl:px-5 xl:py-5 2xl:space-y-6 2xl:px-6 2xl:py-6">
              <div className="space-y-2">
                <Label htmlFor="runName">Run Name</Label>
                <Input
                  id="runName"
                  placeholder="e.g., Medical Images - Analysis 1"
                  value={runName}
                  onChange={(e) => {
                    setRunName(e.target.value);
                    if (errors.name) {
                      setErrors((prev) => ({ ...prev, name: undefined }));
                    }
                  }}
                  disabled={isSubmitting}
                />
                <p className="text-xs text-gray-500 dark:text-gray-400">
                  Give your run a descriptive name to identify it later
                </p>
                {errors.name && <p className="mt-1 text-xs text-red-500 dark:text-red-400">{errors.name}</p>}
              </div>

              <div className="space-y-2">
                <AlgorithmMultiSelector
                  selectedAlgorithms={selectedAlgorithms}
                  onSelectionChange={(algorithms) => {
                    setSelectedAlgorithms(algorithms);
                    if (errors.algorithms) {
                      setErrors((prev) => ({ ...prev, algorithms: undefined }));
                    }
                  }}
                />
                {errors.algorithms && (
                  <p className="mt-1 text-xs text-red-500 dark:text-red-400">{errors.algorithms}</p>
                )}
              </div>
            </div>
          </>
        )}

        {currentStep === 2 && (
          <>
            <div className="border-b border-gray-200 bg-linear-to-r from-cyan-50/50 to-blue-50/50 px-6 py-4 xl:px-5 xl:py-3 2xl:px-6 2xl:py-4 dark:border-gray-800 dark:from-cyan-950/20 dark:to-blue-950/20">
              <h2 className="text-lg font-semibold text-gray-900 xl:text-base 2xl:text-lg dark:text-white">
                Select Images
              </h2>
              <p className="mt-1 text-sm text-gray-600 xl:text-xs 2xl:text-sm dark:text-gray-400">
                Choose which images to process with the selected algorithms
              </p>
            </div>
            <div className="space-y-4 px-6 py-6 lg:space-y-0.5 lg:px-2 lg:py-1 xl:space-y-4 xl:px-5 xl:py-5 2xl:space-y-4 2xl:px-6 2xl:py-6">
              <ImageFilters
                searchQuery={searchQuery}
                onSearchChange={setSearchQuery}
                selectedFormats={selectedFormats}
                selectedSizes={selectedSizes}
                activeFilterCount={activeFilterCount}
                toggleFormat={toggleFormat}
                toggleSize={toggleSize}
                clearAllFilters={clearAllFilters}
                sortControl={<ImageSort sortBy={sortBy} onSortChange={setSortBy} />}
              />

              <div className="flex flex-col gap-2 sm:flex-row sm:items-center sm:justify-between">
                <Tabs
                  value={selectedFilter}
                  onValueChange={(value) => {
                    setSelectedFilter(value as FilterType);
                    setPage(1);
                  }}
                  className="flex-1"
                >
                  <TabsList className="grid h-9 w-full grid-cols-3 xl:h-8 2xl:h-9">
                    {filterOptions
                      .filter((opt) => opt.value === "all" || opt.value === "uploaded" || opt.value === "derived")
                      .map((option) => (
                        <TabsTrigger key={option.value} value={option.value} className="gap-1.5 text-xs 2xl:text-sm">
                          <span className="hidden sm:inline">{option.label}</span>
                          <span className="sm:hidden">{option.label.split(" ")[0]}</span>
                          <Badge
                            variant="secondary"
                            className="h-4 min-w-4 rounded-full px-1.5 text-[10px] font-medium 2xl:h-5 2xl:px-2 2xl:text-xs"
                          >
                            {option.count}
                          </Badge>
                        </TabsTrigger>
                      ))}
                  </TabsList>
                </Tabs>

                <div className="flex items-center gap-3">
                  <div className="flex items-center gap-2 rounded-md border border-gray-200 bg-gray-50 px-3 py-1.5 dark:border-gray-700 dark:bg-gray-800/50">
                    <Checkbox checked={preprocessAllImages} onCheckedChange={(v) => setPreprocessAll(v === true)} />
                    <Tooltip>
                      <TooltipTrigger asChild>
                        <span className="cursor-help text-sm font-medium text-gray-700 dark:text-gray-300">
                          Preprocess All
                        </span>
                      </TooltipTrigger>
                      <TooltipContent sideOffset={6}>
                        Skeletonization algorithms expect binary images containing 0 and 1.
                      </TooltipContent>
                    </Tooltip>
                  </div>

                  {totalPages > 1 && (
                    <ImagePagination currentPage={page} totalPages={totalPages} onPageChange={setPage} />
                  )}
                </div>
              </div>

              <SkeletonizationImageGallery
                images={paginatedImages}
                allFilteredImages={filteredImages}
                selectedImageIds={selectedImageIds}
                onSelectionChange={(ids) => {
                  setSelectedImageIds(ids);
                  if (errors.imageIds) {
                    setErrors((prev) => ({ ...prev, imageIds: undefined }));
                  }
                }}
                getShouldPreprocessImage={getShouldPreprocessImage}
                onTogglePreprocessImage={togglePreprocessForImage}
                selectedFilter={selectedFilter}
              />

              {errors.imageIds && <p className="mt-1 text-xs text-red-500 dark:text-red-400">{errors.imageIds}</p>}
            </div>
          </>
        )}

        {currentStep === 3 && (
          <>
            <div className="border-b border-gray-200 bg-linear-to-r from-cyan-50/50 to-blue-50/50 px-6 py-4 xl:px-5 xl:py-3 2xl:px-6 2xl:py-4 dark:border-gray-800 dark:from-cyan-950/20 dark:to-blue-950/20">
              <h2 className="text-lg font-semibold text-gray-900 xl:text-base 2xl:text-lg dark:text-white">
                Review & Submit
              </h2>
              <p className="mt-1 text-sm text-gray-600 xl:text-xs 2xl:text-sm dark:text-gray-400">
                Confirm your configuration before starting the processing
              </p>
            </div>
            <div className="space-y-6 px-6 py-6 lg:space-y-1 lg:px-2 lg:py-1 xl:space-y-6 xl:px-5 xl:py-5 2xl:space-y-6 2xl:px-6 2xl:py-6">
              <div className="space-y-4">
                <div className="rounded-lg border border-cyan-200 bg-cyan-50/50 p-4 dark:border-cyan-800 dark:bg-cyan-950/20">
                  <h3 className="mb-3 text-sm font-semibold text-gray-900 dark:text-white">Run Configuration</h3>
                  <dl className="space-y-2">
                    <div className="flex justify-between text-sm">
                      <dt className="text-gray-600 dark:text-gray-400">Run Name:</dt>
                      <dd className="font-medium text-gray-900 dark:text-white">{runName}</dd>
                    </div>
                    <div className="flex justify-between text-sm">
                      <dt className="text-gray-600 dark:text-gray-400">Algorithms:</dt>
                      <dd className="font-medium text-gray-900 dark:text-white">{selectedAlgorithms.length}</dd>
                    </div>
                    <div className="flex justify-between text-sm">
                      <dt className="text-gray-600 dark:text-gray-400">Images:</dt>
                      <dd className="font-medium text-gray-900 dark:text-white">{selectedImageIds.length}</dd>
                    </div>
                    <div className="flex justify-between border-t border-cyan-200 pt-2 text-sm dark:border-cyan-800">
                      <dt className="font-semibold text-gray-900 dark:text-white">Total Tasks:</dt>
                      <dd className="font-semibold text-cyan-600 dark:text-cyan-400">
                        {selectedAlgorithms.length * selectedImageIds.length} tasks (1 run)
                      </dd>
                    </div>
                  </dl>
                </div>

                <div className="space-y-2">
                  <h3 className="text-sm font-semibold text-gray-900 dark:text-white">Selected Algorithms</h3>
                  <div className="flex flex-wrap gap-2">
                    {selectedAlgorithms.map((algorithm) => (
                      <Badge
                        key={algorithm}
                        variant="secondary"
                        className="bg-cyan-100 text-cyan-900 dark:bg-cyan-900/30 dark:text-cyan-100"
                      >
                        {algorithm}
                      </Badge>
                    ))}
                  </div>
                </div>

                <div className="rounded-lg border border-gray-200 bg-gray-50 p-4 dark:border-gray-700 dark:bg-gray-800/50">
                  <p className="text-sm text-gray-700 dark:text-gray-300">
                    <strong>Note:</strong> Once submitted, your runs will be queued for processing. You can monitor the
                    progress and view results in the <strong>Lab</strong> page.
                  </p>
                </div>
              </div>
            </div>
          </>
        )}
      </div>

      <div className="flex justify-between">
        <Button onClick={handleBack} variant="outline" disabled={currentStep === 1 || isSubmitting}>
          <ArrowLeft className="mr-2 h-4 w-4" />
          Back
        </Button>

        <div className="flex gap-3">
          {currentStep < STEPS.length ? (
            <Button onClick={handleNext} disabled={!canProceed() || isSubmitting}>
              Next
              <ArrowRight className="ml-2 h-4 w-4" />
            </Button>
          ) : (
            <Button onClick={handleSubmit} disabled={isSubmitting} size="lg" className="gap-2">
              {isSubmitting ? (
                <>
                  <Loader2 className="h-5 w-5 animate-spin" />
                  Submitting...
                </>
              ) : (
                <>
                  <Check className="h-5 w-5" />
                  Submit & Go to Lab
                </>
              )}
            </Button>
          )}
        </div>
      </div>
    </div>
  );
};
