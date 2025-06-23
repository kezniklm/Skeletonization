# CMakeList.txt : SkeletonizationCoreGPU

cmake_minimum_required(VERSION 3.30)
project(SkeletonizationCoreGPU)

include(${CMAKE_CURRENT_SOURCE_DIR}/vcpkg-deps.cmake)

file(GLOB_RECURSE GPU_SOURCES CONFIGURE_DEPENDS source/*.cppm)
file(GLOB_RECURSE GPU_MODULES CONFIGURE_DEPENDS modules/*.ixx)

add_library(SkeletonizationCoreGPU STATIC ${GPU_SOURCES} ${GPU_MODULES})

target_include_directories(SkeletonizationCoreGPU PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
