# CMakeList.txt : SkeletonizationCore

file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

file(GLOB_RECURSE CORE_HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")

add_library(SkeletonizationCore)

add_library(Skeletonization::Core ALIAS SkeletonizationCore)

target_sources(SkeletonizationCore
    PRIVATE
    ${CORE_SOURCES}
    PUBLIC
    ${CORE_HEADERS}
)

target_include_directories(SkeletonizationCore
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

include("${CMAKE_CURRENT_SOURCE_DIR}/vcpkg-deps.cmake")