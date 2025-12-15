# CMakeList.txt : SkeletonizationCoreCPU

include(${CMAKE_CURRENT_SOURCE_DIR}/vcpkg-deps.cmake)

file(GLOB_RECURSE CORE_CPU_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

file(GLOB_RECURSE CORE_CPU_HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")

add_library(SkeletonizationCoreCPU SHARED)

add_library(Skeletonization::CoreCPU ALIAS SkeletonizationCoreCPU)

target_sources(SkeletonizationCoreCPU
    PRIVATE
    ${CORE_CPU_SOURCES}
    PUBLIC
    FILE_SET HEADERS
    BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
    FILES ${CORE_CPU_HEADERS}
)

target_include_directories(SkeletonizationCoreCPU
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(SkeletonizationCoreCPU PUBLIC Skeletonization::Core)
