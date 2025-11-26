# CMakeList.txt : SkeletonizationCLI

cmake_minimum_required(VERSION 3.30)
project(SkeletonizationCLI)

file(GLOB_RECURSE CLI_SOURCE CONFIGURE_DEPENDS source/*.ixx)
file(GLOB_RECURSE CLI_MAIN CONFIGURE_DEPENDS main.cpp)

include(${CMAKE_CURRENT_SOURCE_DIR}/vcpkg-deps.cmake)

file(COPY "${CMAKE_SOURCE_DIR}/skeletonizer_config.json" DESTINATION "${CMAKE_BINARY_DIR}")

add_executable(SkeletonizationCLI ${CLI_MAIN} ${CLI_SOURCE})

target_sources(SkeletonizationCLI
    PUBLIC
    FILE_SET modules TYPE CXX_MODULES FILES
    ${CLI_SOURCE})

target_include_directories(SkeletonizationCLI PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(SkeletonizationCLI PRIVATE SkeletonizationCore SkeletonizationCoreCPU SkeletonizationCoreMT SkeletonizationVisualizer benchmark::benchmark CLI11::CLI11 RapidJSON rapidjson glog::glog Drogon::Drogon)

if(SKELETONIZATION_ENABLE_GPU)
    target_link_libraries(SkeletonizationCLI PRIVATE SkeletonizationCoreGPU)
endif()
