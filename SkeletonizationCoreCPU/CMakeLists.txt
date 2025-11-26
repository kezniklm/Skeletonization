# CMakeList.txt : SkeletonizationCoreCPU

cmake_minimum_required(VERSION 3.30)
project(SkeletonizationCoreCPU)

include(${CMAKE_CURRENT_SOURCE_DIR}/vcpkg-deps.cmake)

file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS source/*.ixx)

add_library(SkeletonizationCoreCPU)

target_sources(SkeletonizationCoreCPU
    PUBLIC
    FILE_SET modules TYPE CXX_MODULES FILES
    ${CORE_SOURCES})

target_link_libraries(SkeletonizationCoreCPU PUBLIC SkeletonizationCore)
