# CMakeList.txt : SkeletonizationCoreMT

include(${CMAKE_CURRENT_SOURCE_DIR}/vcpkg-deps.cmake)

file(GLOB_RECURSE CORE_THREAD_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

file(GLOB_RECURSE CORE_THREAD_HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")

add_library(SkeletonizationCoreMT SHARED)

add_library(Skeletonization::CoreMT ALIAS SkeletonizationCoreMT)

target_sources(SkeletonizationCoreMT
    PRIVATE
    ${CORE_THREAD_SOURCES}
    PUBLIC
    FILE_SET HEADERS
    BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
    FILES ${CORE_THREAD_HEADERS}
)

target_include_directories(SkeletonizationCoreMT
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(SkeletonizationCoreMT PUBLIC Skeletonization::Core)
