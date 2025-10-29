# CMakeList.txt : SkeletonizationWorker

cmake_minimum_required(VERSION 3.30)
project(SkeletonizationWorker)

file(GLOB_RECURSE WORKER_SOURCE CONFIGURE_DEPENDS source/*.ixx)
file(GLOB_RECURSE WORKER_MAIN CONFIGURE_DEPENDS main.cpp)

include(${CMAKE_CURRENT_SOURCE_DIR}/vcpkg-deps.cmake)

file(COPY "${CMAKE_SOURCE_DIR}/skeletonizer_config.json" DESTINATION "${CMAKE_BINARY_DIR}")

add_executable(SkeletonizationWorker ${WORKER_MAIN} ${WORKER_SOURCE})

target_sources(SkeletonizationWorker
    PUBLIC
    FILE_SET modules TYPE CXX_MODULES FILES
    ${WORKER_SOURCE})

target_include_directories(SkeletonizationWorker PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(SkeletonizationWorker PRIVATE SkeletonizationCore RapidJSON rapidjson glog::glog hiredis::hiredis)

if(SKELETONIZATION_ENABLE_GPU)
    target_link_libraries(SkeletonizationWorker PRIVATE SkeletonizationCoreGPU)
endif()
