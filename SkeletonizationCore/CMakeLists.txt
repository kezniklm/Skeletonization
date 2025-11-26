# CMakeList.txt : SkeletonizationCore

cmake_minimum_required(VERSION 3.30)
project(SkeletonizationCore)

file(GLOB_RECURSE CoreSources source/*.ixx)

add_library(SkeletonizationCore)

target_sources(SkeletonizationCore
        PUBLIC
        FILE_SET modules TYPE CXX_MODULES FILES
        ${CoreSources})

include(${CMAKE_CURRENT_SOURCE_DIR}/vcpkg-deps.cmake)
