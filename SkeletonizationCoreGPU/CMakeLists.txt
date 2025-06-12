# CMakeList.txt : SkeletonizationCoreGPU

cmake_minimum_required(VERSION 3.30)
project(SkeletonizationCoreGPU)

include(${CMAKE_CURRENT_SOURCE_DIR}/vcpkg-deps.cmake)

file(GLOB_RECURSE GPU_SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE GPU_HEADERS CONFIGURE_DEPENDS include/*.h include/*.hpp)

add_library(SkeletonizationCoreGPU STATIC ${GPU_SOURCES} ${GPU_HEADERS})

target_include_directories(SkeletonizationCoreGPU PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
