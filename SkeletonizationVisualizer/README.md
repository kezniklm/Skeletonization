# SkeletonizationVisualizer

A modern, interactive web application for visualizing and analyzing skeletonization algorithm benchmarks. Built with React, TypeScript, and Tailwind CSS.

## Overview

The SkeletonizationVisualizer is a comprehensive web-based tool designed to display benchmark results from various skeletonization algorithms. It provides real-time visualization of algorithm performance metrics, image comparisons, and detailed statistical analysis to help researchers and developers evaluate the effectiveness of different skeletonization implementations.

## Features

### 🖼️ Interactive Image Gallery

- **Real-time Data Loading**: Automatically refreshes benchmark data every 2 seconds
- **Dynamic Image Display**: View original and skeletonized images side-by-side
- **Image Navigation**: Navigate through images using keyboard shortcuts or UI controls
- **Zoom and Pan**: Interactive image viewer with zoom capabilities (0.1x to 10x)

### 📊 Performance Analytics

- **Benchmark Metrics**: Display execution time, memory usage, and throughput statistics
- **Algorithm Comparison**: Compare performance across different algorithms
- **Statistical Dashboard**: View min, max, average execution times and standard deviation
- **Complexity Analysis**: Algorithm complexity information (O(n), O(n²), etc.)

### 🔍 Advanced Comparison Tools

- **Side-by-Side Comparison**: Interactive slider to compare original vs. skeletonized images
- **Multi-Algorithm Comparison**: Compare results from different algorithms on the same input
- **Image Modal View**: Full-screen image viewing with detailed metrics
- **Export Functionality**: Download individual images or export entire result sets

### 🎨 User Experience

- **Dark/Light Theme**: Toggle between themes for comfortable viewing
- **Search and Filter**: Find specific algorithms or image sets quickly
- **Responsive Design**: Works seamlessly on desktop, tablet, and mobile devices
- **Loading States**: Smooth loading indicators and error handling

## Technology Stack

- **Framework**: React 19 with TypeScript
- **Styling**: Tailwind CSS 4.x with custom animations
- **Build Tool**: Vite 6.x
- **Code Quality**: ESLint, Prettier with comprehensive rules
- **File Handling**: JSZip for batch exports

## Installation

### Prerequisites

- Node.js 18+
- npm or yarn package manager

### Setup

1. **Clone the repository** (if not already done):

   ```bash
   git clone <repository-url>
   cd Skeletonization/SkeletonizationVisualizer
   ```

2. **Install dependencies**:

   ```bash
   npm install
   # or
   yarn install
   ```

3. **Prepare benchmark data**:
   Place your `benchmark_data.json` file in the `public` directory. The file should follow this structure:
   ```json
   {
     "containers": [
       {
         "name": "Algorithm Name",
         "images": [
           {
             "id": "unique-id",
             "label": "Image Label",
             "data": "base64-encoded-image-data",
             "width": 512,
             "height": 512,
             "metrics": {
               "executionTimeMs": 150.5,
               "memoryUsageMB": 45.2,
               "pixelCount": 262144,
               "nonZeroPixels": 45123
             }
           }
         ],
         "algorithmInfo": {
           "description": "Algorithm description",
           "avgTime": 150.5,
           "minTime": 120.0,
           "maxTime": 200.0,
           "complexity": "O(n)"
         }
       }
     ],
     "timestamp": "2024-01-01T00:00:00.000Z"
   }
   ```

## Usage

### Development Mode

```bash
npm run dev
# or
yarn dev
```

The application will be available at `http://localhost:5173`

### Production Build

```bash
npm run build
# or
yarn build
```

### Preview Production Build

```bash
npm run preview
# or
yarn preview
```

### Linting

```bash
npm run lint
# or
yarn lint
```

## Configuration

### Application Settings

The application can be configured through `src/constants.ts`:

```typescript
export const APP_CONFIG = {
  DATA_REFRESH_INTERVAL: 2000, // Data refresh interval in milliseconds
  SLIDESHOW_INTERVAL: 3000, // Slideshow interval in milliseconds
  DEFAULT_SLIDER_POSITION: 50 // Default comparison slider position
};
```

### Build Configuration

- **Vite Config**: `vite.config.ts` - Build and development server settings
- **TypeScript**: `tsconfig.json` - TypeScript compiler options
- **Tailwind**: `postcss.config.js` - CSS processing configuration

## Project Structure

```
src/
├── components/           # Reusable UI components
│   ├── modals/          # Modal components (Image, Comparison)
│   ├── ErrorBoundary.tsx
│   ├── ImageCard.tsx
│   ├── LoadingSpinner.tsx
│   └── ...
├── contexts/            # React contexts
│   └── ThemeContext.tsx
├── hooks/               # Custom React hooks
│   ├── useBenchmarkData.ts
│   ├── useImageModal.ts
│   └── ...
├── pages/               # Page components
│   └── SkeletonizerBenchmarkVisualizer.tsx
├── constants.ts         # Application constants
├── types.ts            # TypeScript type definitions
├── utils.ts            # Utility functions
└── main.tsx            # Application entry point
```

## Key Components

### BenchmarkData Hook

Handles automatic data loading and refresh:

```typescript
const { data, loading, error } = useBenchmarkData();
```

### Image Modal System

Interactive image viewing with navigation:

```typescript
const { selectedImage, openImageModal, closeImageModal } = useImageModal();
```

### Comparison System

Side-by-side algorithm comparison:

```typescript
const { comparisonMode, openComparison, sliderPosition } = useComparisonModal();
```

### Theme Management

Dark/light theme switching:

```typescript
const { toggleTheme, getThemeClasses } = useTheme();
```

## Data Format Requirements

The visualizer expects benchmark data in a specific JSON format with the following structure:

- **BenchmarkData**: Root object containing timestamp and containers
- **ImageContainer**: Groups of images from the same algorithm
- **ImageData**: Individual image with metrics and base64 data
- **BenchmarkMetrics**: Performance measurements (time, memory, etc.)
- **AlgorithmStats**: Statistical analysis of algorithm performance

## Integration with Skeletonization Core

This visualizer is designed to work with the main Skeletonization project:

1. **SkeletonizationCLI** generates benchmark data
2. **SkeletonizationCore** processes the algorithms
3. **SkeletonizationVisualizer** displays the results

The integration workflow:

1. Run skeletonization benchmarks via CLI
2. CLI outputs `benchmark_data.json`
3. Place the JSON file in the visualizer's public directory
4. Launch the visualizer to view results

## Browser Support

- Chrome 90+
- Firefox 88+
- Safari 14+
- Edge 90+

## Contributing

1. Follow the existing code style (enforced by ESLint/Prettier)
2. Add TypeScript types for new features
3. Test changes across different screen sizes
4. Update documentation for new features

## License

This project is part of the larger Skeletonization project. Please refer to the main project license.

## Related Projects

- **SkeletonizationCore**: Core algorithm implementations
- **SkeletonizationCLI**: Command-line interface for running benchmarks
- **SkeletonizationCoreGPU**: GPU-accelerated implementations

For more information about the overall project, see the [main README](../README.md).
