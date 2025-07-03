# CMakeList.txt : SkeletonizationCoreGPU

cmake_minimum_required(VERSION 3.30)
project(SkeletonizationCoreGPU)

include(${CMAKE_CURRENT_SOURCE_DIR}/vcpkg-deps.cmake)

file(GLOB_RECURSE GPU_SOURCES CONFIGURE_DEPENDS source/*.ixx)

add_library(SkeletonizationCoreGPU)

target_sources(SkeletonizationCoreGPU
        PUBLIC
        FILE_SET modules TYPE CXX_MODULES FILES
        ${GPU_SOURCES} )

target_link_libraries(SkeletonizationCoreGPU PUBLIC SkeletonizationCoreCommon)