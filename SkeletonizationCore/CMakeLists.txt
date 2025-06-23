# CMakeList.txt : SkeletonizationCore

cmake_minimum_required(VERSION 3.30)
project(SkeletonizationCore)

include(${CMAKE_CURRENT_SOURCE_DIR}/vcpkg-deps.cmake)

file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS source/*.cppm)
file(GLOB_RECURSE CORE_MODULES CONFIGURE_DEPENDS modules/*.ixx)

add_library(SkeletonizationCore STATIC ${CORE_SOURCES} ${CORE_MODULES})

target_include_directories(SkeletonizationCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
