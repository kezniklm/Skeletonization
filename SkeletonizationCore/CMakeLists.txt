# CMakeList.txt : SkeletonizationCore

cmake_minimum_required(VERSION 3.30)
project(SkeletonizationCore)

include(${CMAKE_CURRENT_SOURCE_DIR}/vcpkg-deps.cmake)

file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS source/*.ixx)

add_library(SkeletonizationCore)

target_sources(SkeletonizationCore
    PUBLIC
    FILE_SET modules TYPE CXX_MODULES FILES
    ${CORE_SOURCES})

target_link_libraries(SkeletonizationCore PUBLIC SkeletonizationCoreCommon)

add_subdirectory(
    ${CMAKE_SOURCE_DIR}/SkeletonizationCoreCommon
    ${CMAKE_BINARY_DIR}/SkeletonizationCoreCommon-build
)
