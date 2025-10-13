# CMakeList.txt : SkeletonizationPython

cmake_minimum_required(VERSION 3.30)
project(SkeletonizationPython)

include(${CMAKE_CURRENT_SOURCE_DIR}/vcpkg-deps.cmake)

file(GLOB_RECURSE PYTHON_WRAPPER_SOURCES CONFIGURE_DEPENDS source/*.cpp)

target_sources(SkeletonizationPython PRIVATE ${PYTHON_WRAPPER_SOURCES})

target_include_directories(SkeletonizationPython PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(SkeletonizationPython PRIVATE SkeletonizationCore)

if(SKELETONIZATION_ENABLE_GPU)
    target_link_libraries(SkeletonizationPython PRIVATE SkeletonizationCoreGPU)
endif()

set_target_properties(SkeletonizationPython PROPERTIES OUTPUT_NAME skeletonization)
