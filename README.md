# Skeletonization

## Overview
This project is part of diploma thesis **Analysis and Efficient Implementation of Skeletonization Algorithms for Digital Image Processing** – the process of reducing shapes in images to their simplified skeletal form while preserving structural and topological properties.  
The goal is to **implement, optimize, and compare skeletonization algorithms** across different computing paradigms:
- **Single-threaded CPU implementation**
- **Multi-threaded CPU implementation**
- **GPU-accelerated implementation** (CUDA / OpenCL)

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
├── SkeletonizationCore/         # Core skeletonization logic
├── SkeletonizationCoreCommon/   # Shared utilities, configs, and helpers
├── SkeletonizationCoreGPU/      # GPU accelerated code (CUDA / OpenCL)
├── SkeletonizationCLI/          # Command-line interface for running experiments
├── SkeletonizationPython/       # Python bindings / scripts
├── docs/                        # Documentation and thesis-related notes (optional)
├── out/                         # Build / output directory
├── .gitattributes
├── .gitignore
├── CMakeLists.txt
├── CMakePresets.json
├── LICENSE.txt
├── README.md
├── skeletonizer_config.json     # Default config file
├── vcpkg.json                   # vcpkg package dependencies
├── vcpkg-configuration.json
```

## Requirements
- **C++23 compiler** (MSVC, Clang, or GCC)
- **CMake ≥ 3.30**
- **vcpkg** (for dependency management)
- **OpenCV ≥ 4.0** (image I/O and visualization)
- **CUDA Toolkit ≥ 11.0** or **OpenCL 2.0+** (for GPU acceleration)
- **Python ≥ 3.12** (if using `SkeletonizationPython`)

## Building with CMake Presets

This project provides ready-to-use **CMake presets** (`CMakePresets.json`) for **Windows** and **Linux**.

### Base Presets
- **`windows-base`** – Base preset for Windows (MSVC, Ninja).
- **`linux-base`** – Base preset for Linux (GCC, Ninja).

### Derived Presets
- **`x64-debug`** – Standard Debug build.
- **`x64-release`** – Optimized Release build.

All presets:
- Output build artifacts to `out/build/<preset>` and install into `out/install/<preset>`.
- Automatically configure compilers depending on host OS.
- Integrate with **vcpkg** via `$env{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake`.

### Example Usage

Configure with a preset:
```
cmake --preset=x64-release
```

Build:
```
cmake --build --preset=x64-release
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
- `--benchmark_out=<filename>` – Specify the output filename for benchmark results.
- `--benchmark_out_format=<type>` – Export results in specified format - type can be json and csv.

## License
See [LICENSE.txt](LICENSE.txt) for details.  
This project is released under the **MIT License**.
