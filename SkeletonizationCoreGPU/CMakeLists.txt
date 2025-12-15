# CMakeList.txt : SkeletonizationCoreGPU

if(NOT SKELETONIZATION_ENABLE_GPU)
    return()
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/vcpkg-deps.cmake)

file(GLOB_RECURSE GPU_CPP_FILES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

file(GLOB_RECURSE GPU_CUDA_FILES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cu"
)

file(GLOB_RECURSE GPU_HEADER_FILES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.cuh"
)

add_library(SkeletonizationCoreGPU SHARED)

add_library(Skeletonization::CoreGPU ALIAS SkeletonizationCoreGPU)

target_sources(SkeletonizationCoreGPU
    PRIVATE
    ${GPU_CPP_FILES}
    ${GPU_CUDA_FILES}
    PUBLIC
    FILE_SET HEADERS
    BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
    FILES ${GPU_HEADER_FILES}
)

target_include_directories(SkeletonizationCoreGPU
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

set_target_properties(SkeletonizationCoreGPU PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS OFF

    CUDA_STANDARD 20
    CUDA_STANDARD_REQUIRED YES
    CUDA_EXTENSIONS OFF
)

target_compile_options(SkeletonizationCoreGPU
    PRIVATE
    $<$<COMPILE_LANGUAGE:CUDA>:-Wno-deprecated-gpu-targets>
)

target_link_libraries(SkeletonizationCoreGPU
    PUBLIC
    Skeletonization::Core
)
