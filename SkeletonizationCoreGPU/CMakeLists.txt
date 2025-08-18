# CMakeList.txt : SkeletonizationCoreGPU

cmake_minimum_required(VERSION 3.30)
project(SkeletonizationCoreGPU)

include(${CMAKE_CURRENT_SOURCE_DIR}/vcpkg-deps.cmake)

file(GLOB_RECURSE GPU_MODULES CONFIGURE_DEPENDS source/*.ixx )

file(GLOB_RECURSE GPU_CUDA_FILES CONFIGURE_DEPENDS source/*.cu )

set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

add_library(SkeletonizationCoreGPU)

target_sources(SkeletonizationCoreGPU
    PUBLIC
    FILE_SET modules TYPE CXX_MODULES FILES
    ${GPU_MODULES}
    PRIVATE
    ${GPU_CUDA_FILES}
)

target_link_libraries(SkeletonizationCoreGPU PUBLIC SkeletonizationCoreCommon)