# CMakeList.txt : SkeletonizationCLI

include("${CMAKE_CURRENT_SOURCE_DIR}/vcpkg-deps.cmake")

file(GLOB_RECURSE CLI_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
)

file(GLOB_RECURSE CLI_HEADERS
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)

find_path(BEXT_DI_INCLUDE_DIRS "boost/di.hpp")

add_executable(SkeletonizationCLI)

target_sources(SkeletonizationCLI
    PRIVATE
    ${CLI_SOURCES}
    PRIVATE
    FILE_SET HEADERS
    BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
    FILES ${CLI_HEADERS}
)

file(COPY "${CMAKE_SOURCE_DIR}/skeletonizer_config.json" DESTINATION "${CMAKE_BINARY_DIR}")

target_include_directories(SkeletonizationCLI
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    ${BEXT_DI_INCLUDE_DIRS}
)

target_link_libraries(SkeletonizationCLI
    PRIVATE
    SkeletonizationCore
    SkeletonizationCoreCPU
    SkeletonizationCoreMT
    SkeletonizationVisualizer
    benchmark::benchmark
    CLI11::CLI11
    RapidJSON
    rapidjson
    Drogon::Drogon)

if(SKELETONIZATION_ENABLE_GPU)
    target_link_libraries(SkeletonizationCLI PRIVATE SkeletonizationCoreGPU)
endif()