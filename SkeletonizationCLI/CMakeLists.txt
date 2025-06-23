# CMakeList.txt : SkeletonizationCLI

cmake_minimum_required(VERSION 3.30)
project(SkeletonizationCLI)

file(GLOB_RECURSE CLI_SOURCES CONFIGURE_DEPENDS source/*.cppm)
file(GLOB_RECURSE CLI_MODULES CONFIGURE_DEPENDS modules/*.ixx)
file(GLOB_RECURSE CLI_MAIN CONFIGURE_DEPENDS main.cpp)

include(${CMAKE_CURRENT_SOURCE_DIR}/vcpkg-deps.cmake)

add_executable(SkeletonizationCLI ${CLI_MAIN} ${CLI_SOURCES} ${CLI_MODULES})

target_include_directories(SkeletonizationCLI PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(SkeletonizationCLI PRIVATE SkeletonizationCore SkeletonizationCoreGPU benchmark::benchmark)
