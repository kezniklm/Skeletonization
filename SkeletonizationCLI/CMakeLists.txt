# CMakeList.txt : SkeletonizationCLI

cmake_minimum_required(VERSION 3.30)
project(SkeletonizationCLI)

file(GLOB_RECURSE CLI_SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE CLI_HEADERS CONFIGURE_DEPENDS include/*.h include/*.hpp)
file(GLOB_RECURSE CLI_MAIN CONFIGURE_DEPENDS main.cpp)

include(${CMAKE_CURRENT_SOURCE_DIR}/vcpkg-deps.cmake)

add_executable(SkeletonizationCLI ${CLI_MAIN} ${CLI_SOURCES} ${CLI_HEADERS})

target_include_directories(SkeletonizationCLI PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(SkeletonizationCLI PRIVATE SkeletonizationCore SkeletonizationCoreGPU benchmark::benchmark)
