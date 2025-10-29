# CMakeList.txt : Global project for Skeletonization

cmake_minimum_required(VERSION 3.30)

project(Skeletonization)

# Enable Hot Reload for MSVC compilers if supported.
if(POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Set IS_64BIT flag
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(IS_64BIT TRUE)
else()
    set(IS_64BIT FALSE)
endif()

# Enable CUDA debugging
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Enabling CUDA debugging")

    if(CMAKE_CUDA_COMPILER)
        add_compile_options($<$<COMPILE_LANGUAGE:CUDA>:-G>)
        add_compile_options($<$<COMPILE_LANGUAGE:CUDA>:-g>)
    endif()
endif()

# Enable performance optimizations
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Enabling Release optimizations for performance")

    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-O3 -march=native>)
    elseif(MSVC)
        add_compile_options($<$<COMPILE_LANGUAGE:CXX>:/arch:AVX2>)
        add_compile_options($<$<COMPILE_LANGUAGE:CXX>:/O2>)
    endif()

    if(CMAKE_CUDA_COMPILER)
        add_compile_options($<$<COMPILE_LANGUAGE:CUDA>:-O3>)
        add_compile_options($<$<COMPILE_LANGUAGE:CUDA>:--use_fast_math>)
        add_compile_options($<$<COMPILE_LANGUAGE:CUDA>:--restrict>)
    endif()
endif()

# Set C++ standards
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

add_compile_definitions(SKELETONIZATION_WITH_GPU=$<BOOL:${SKELETONIZATION_ENABLE_GPU}>)

add_subdirectory(SkeletonizationCore)

if(SKELETONIZATION_ENABLE_GPU)
    find_package(CUDAToolkit REQUIRED)

    message(STATUS "CUDA preset requested and CUDA found: enabling GPU support")
    add_compile_definitions(SKELETONIZATION_WITH_GPU=1)
    add_subdirectory(SkeletonizationCoreGPU)
else()
    message(STATUS "CUDA preset not requested: disabling GPU support")
    add_compile_definitions(SKELETONIZATION_WITH_GPU=0)
endif()

add_subdirectory(SkeletonizationPython)
add_subdirectory(SkeletonizationVisualizer)
add_subdirectory(SkeletonizationTests)
add_subdirectory(SkeletonizationWorker)
add_subdirectory(SkeletonizationCLI)
