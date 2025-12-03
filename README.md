# Skeletonization

[![CI - SkeletonizationWebApplication](https://github.com/kezniklm/Skeletonization/actions/workflows/CI-SkeletonizationWebApplication.yml/badge.svg?branch=master)](https://github.com/kezniklm/Skeletonization/actions/workflows/CI-SkeletonizationWebApplication.yml)
[![CI - SkeletonizationVisualizer](https://github.com/kezniklm/Skeletonization/actions/workflows/CI-SkeletonizationVisualizer.yml/badge.svg?branch=master)](https://github.com/kezniklm/Skeletonization/actions/workflows/CI-SkeletonizationVisualizer.yml)
[![CI - C++](https://github.com/kezniklm/Skeletonization/actions/workflows/CI-C++.yml/badge.svg?branch=master)](https://github.com/kezniklm/Skeletonization/actions/workflows/CI-C++.yml)

## Overview

This project is part of diploma thesis **Analysis and Efficient Implementation of Skeletonization Algorithms for Digital Image Processing** – the process of reducing shapes in images to their simplified skeletal form while preserving structural and topological properties.  
The goal is to **implement, optimize, and compare skeletonization algorithms** across different computing paradigms:

-   **Single-threaded CPU implementation**
-   **Multi-threaded CPU implementation**
-   **GPU-accelerated implementation**

## Objectives

1. Implement multiple skeletonization algorithms.
2. Evaluate computational performance across:
    - Sequential (single-threaded) execution.
    - Parallel execution on multi-core CPUs.
    - Massively parallel execution on GPUs.
3. Compare the results in terms of:
    - Execution time.
    - Scalability.
    - Accuracy of skeletonization results.
4. Provide a reproducible benchmarking framework.

## Repository Structure

```
Skeletonization/
├── SkeletonizationCLI/              # Command-line interface for running experiments
├── SkeletonizationCore/             # Core skeletonization logic
├── SkeletonizationCoreCPU/          # Single-threaded CPU implementations
├── SkeletonizationCoreGPU/          # GPU accelerated code (CUDA)
├── SkeletonizationCoreMT/           # Multi-threaded CPU implementations
├── SkeletonizationPython/           # Python bindings
├── SkeletonizationTests/            # Unit and integration tests
├── SkeletonizationVisualizer/       # Web-based visualization tool (Vite + React)
├── SkeletonizationWebApplication/   # Full-stack web application (Next.js)
├── SkeletonizationWorker/           # Background worker service
├── CMakeLists.txt
├── CMakePresets.json
├── Folder.DotSettings
├── LICENSE.txt
├── README.md
├── skeletonizer_config.json         # Default config file
├── vcpkg.json                       # vcpkg package dependencies
├── vcpkg-configuration.json
```

## Requirements

-   **C++23 compiler** (MSVC, Clang, or GCC)
-   **CMake ≥ 3.30**
-   **vcpkg** (for dependency management)
-   **OpenCV ≥ 4.0** (image I/O and visualization)
-   **CUDA Toolkit ≥ 12.9** (for GPU acceleration)
-   **Python ≥ 3.12** (if using `SkeletonizationPython`)

## Building with CMake Presets

This repository ships a `CMakePresets.json` that defines platform-specific base presets and several derived presets, including CUDA-enabled variants. Presets place build artifacts under `out/build/<preset>` and install artifacts under `out/install/<preset>`.

### Presets Included

- **Base (hidden):** `windows-base`, `linux-base` (MSVC/GCC + Ninja).
- **Release:** `x64-release-windows`, `x64-release-linux`.
- **Debug:** `x64-debug-windows`, `x64-debug-linux`.
- **CUDA-enabled variants:** `x64-release-windows-cuda`, `x64-debug-windows-cuda`, `x64-release-linux-cuda`, `x64-debug-linux-cuda`.

These CUDA presets set `VCPKG_MANIFEST_FEATURES=cuda` and `SKELETONIZATION_ENABLE_GPU=ON` so GPU targets are compiled when available.

### Example Commands (PowerShell)

Configure (Release, Windows CUDA):

```powershell
cmake --preset=x64-release-windows-cuda
```

Build (use the corresponding build preset):

```powershell
cmake --build --preset=x64-release-windows-cuda
```

Run tests for a preset:

```powershell
cmake --preset=x64-debug-windows-cuda --build
ctest --build-config Debug --test-dir out/build/x64-debug-windows-cuda
```

## Running the CLI

Examples of running different implementations:

### Single-threaded CPU

```bash
./SkeletonizationCLI --leaf-name sample --leaf-path input.png --skeleton cpu:zhang-suen
```

### Multi-threaded CPU

```bash
./SkeletonizationCLI --leaf-name sample --leaf-path input.png --skeleton thread:guo-hall
```

### GPU

```bash
./SkeletonizationCLI --leaf-name sample --leaf-path input.png --skeleton gpu:zhang-suen
```

### With Config File

```bash
./SkeletonizationCLI --config skeletonizer_config.json
```

## Benchmark output

By default, benchmark results are generated in JSON format:

-   `--benchmark_out=<file_path>` – Specify the output filename for benchmark results.

## License

See [LICENSE.txt](LICENSE.txt) for details.  
This project is released under the **MIT License**.
