# CMakeList.txt : SkeletonizationCoreCommon

cmake_minimum_required(VERSION 3.30)
project(SkeletonizationCoreCommon)

file(GLOB_RECURSE CoreCommonSources source/*.ixx)

add_library(SkeletonizationCoreCommon)

target_sources(SkeletonizationCoreCommon
        PUBLIC
        FILE_SET modules TYPE CXX_MODULES FILES
        ${CoreCommonSources})

include(${CMAKE_CURRENT_SOURCE_DIR}/vcpkg-deps.cmake)

target_include_directories(SkeletonizationCoreCommon PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/modules)
