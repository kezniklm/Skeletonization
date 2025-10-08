import { useState, useCallback } from "react";

import type { ImageContainer } from "../types";
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
    comparisonMode,
    comparisonImages,
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

  const handleFiltersChange = useCallback((containers: ImageContainer[]) => {
    setFilteredContainers(containers);
  }, []);

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
        onCompare={openComparison}
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

      {comparisonImages[0] && comparisonImages[1] && (
        <ComparisonModal
          original={comparisonImages[0]}
          processed={comparisonImages[1]}
          isOpen
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
