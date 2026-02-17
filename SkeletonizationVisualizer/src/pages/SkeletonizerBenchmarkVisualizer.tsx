import { useState, useCallback } from "react";

import type { ImageContainer, ImageData } from "../types";
import { StatsDashboard } from "../components/StatsComponents";
import { downloadImage, exportAllImages } from "../utils";
import { useBenchmarkData } from "../hooks/useBenchmarkData";
import { useImageModal } from "../hooks/useImageModal";
import { useComparisonModal } from "../hooks/useComparisonModal";
import { useImageNavigation } from "../hooks/useImageNavigation";
import { MainContent } from "../components/MainContent";
import { ComparisonModal } from "../components/modals/ComparisonModal";
import { ImageModal } from "../components/modals/ImageModal";
import { Navigation } from "../components/Navigation";
import { SearchAndFilter } from "../components/SearchAndFilter";
import { ErrorDisplay } from "../components/ErrorDisplay";
import { LoadingSpinner } from "../components/LoadingSpinner";
import { useTheme } from "../contexts/ThemeContext";

const SkeletonizerBenchmarkVisualizer = () => {
  const { toggleTheme, getThemeClasses } = useTheme();

  const { data, loading, error } = useBenchmarkData();

  const { selectedImage, selectedContainer, openImageModal, closeImageModal, navigateToImage } = useImageModal();

  const {
    isOpen: isComparisonOpen,
    comparisonMode,
    comparisonOriginal,
    comparisonProcessed,
    sliderPosition,
    setComparisonMode,
    setSliderPosition,
    openComparison,
    closeComparison
  } = useComparisonModal();

  const { navigateImage } = useImageNavigation({
    data,
    selectedImage,
    onImageSelect: navigateToImage
  });

  const [filteredContainers, setFilteredContainers] = useState<ImageContainer[]>([]);
  const [selectedComparisonIds, setSelectedComparisonIds] = useState<Record<string, string[]>>({});

  const handleFiltersChange = useCallback((containers: ImageContainer[]) => {
    setFilteredContainers(containers);
  }, []);

  const toggleComparisonSelection = useCallback((containerName: string, image: ImageData) => {
    setSelectedComparisonIds((prev) => {
      const existing = prev[containerName] ?? [];
      const isSelected = existing.includes(image.id);
      const next = isSelected ? existing.filter((id) => id !== image.id) : [...existing, image.id];

      if (next.length === 0) {
        const { [containerName]: _removed, ...rest } = prev;
        return rest;
      }

      return {
        ...prev,
        [containerName]: next
      };
    });
  }, []);

  const clearComparisonSelection = useCallback((containerName: string) => {
    setSelectedComparisonIds((prev) => {
      const { [containerName]: _removed, ...rest } = prev;
      return rest;
    });
  }, []);

  const selectAllComparisonImages = useCallback((container: ImageContainer) => {
    const selectedIds = container.images.slice(1).map((image) => image.id);

    setSelectedComparisonIds((prev) => {
      if (selectedIds.length === 0) {
        const { [container.name]: _removed, ...rest } = prev;
        return rest;
      }

      return {
        ...prev,
        [container.name]: selectedIds
      };
    });
  }, []);

  const openSelectedComparison = useCallback(
    (container: ImageContainer) => {
      const [original, ...processedCandidates] = container.images;
      if (!original) {
        return;
      }

      const selectedIds = selectedComparisonIds[container.name] ?? [];
      const selectedProcessed = processedCandidates.filter((image) => selectedIds.includes(image.id));

      if (!selectedProcessed.length) {
        return;
      }

      openComparison(original, selectedProcessed);
    },
    [openComparison, selectedComparisonIds]
  );

  if (loading) {
    return (
      <div className={`min-h-screen ${getThemeClasses().bg} flex items-center justify-center`}>
        <LoadingSpinner message="Loading benchmark data..." size="lg" />
      </div>
    );
  }

  if (error || !data) {
    return (
      <div className={`min-h-screen ${getThemeClasses().bg} flex items-center justify-center px-4`}>
        <ErrorDisplay
          title="Error Loading Data"
          message={
            error ?? "No data available. Make sure the C++ application has generated the benchmark_data.json file."
          }
          retry={() => window.location.reload()}
        />
      </div>
    );
  }

  return (
    <div className={`min-h-screen ${getThemeClasses().bg}`}>
      <Navigation data={data} onExportAll={() => exportAllImages(data)} onToggleTheme={toggleTheme} />

      <StatsDashboard data={data} />

      <SearchAndFilter containers={data.containers} onFiltersChange={handleFiltersChange} />

      <MainContent
        filteredContainers={filteredContainers}
        onImageClick={openImageModal}
        onDownload={downloadImage}
        selectedComparisonIds={selectedComparisonIds}
        onToggleComparisonSelection={toggleComparisonSelection}
        onClearComparisonSelection={clearComparisonSelection}
        onSelectAllComparison={selectAllComparisonImages}
        onCompareSelected={openSelectedComparison}
      />

      {selectedImage && data && (
        <ImageModal
          image={selectedImage}
          container={selectedContainer}
          isOpen
          onClose={closeImageModal}
          onNavigate={navigateImage}
          onDownload={downloadImage}
        />
      )}

      {isComparisonOpen && comparisonOriginal && comparisonProcessed.length > 0 && (
        <ComparisonModal
          original={comparisonOriginal}
          processedImages={comparisonProcessed}
          isOpen={isComparisonOpen}
          onClose={closeComparison}
          comparisonMode={comparisonMode}
          onComparisonModeChange={setComparisonMode}
          sliderPosition={sliderPosition}
          onSliderPositionChange={setSliderPosition}
        />
      )}
    </div>
  );
};

export default SkeletonizerBenchmarkVisualizer;
