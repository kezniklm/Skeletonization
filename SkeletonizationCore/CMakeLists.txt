# CMakeList.txt : SkeletonizationCore

cmake_minimum_required(VERSION 3.30)
project(SkeletonizationCore)

include(${CMAKE_CURRENT_SOURCE_DIR}/vcpkg-deps.cmake)

file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE CORE_HEADERS CONFIGURE_DEPENDS include/*.h include/*.hpp)

add_library(SkeletonizationCore STATIC ${CORE_SOURCES} ${CORE_HEADERS})

target_include_directories(SkeletonizationCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
