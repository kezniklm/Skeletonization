# CMakeList.txt : SkeletonizationWorker

include("${CMAKE_CURRENT_SOURCE_DIR}/vcpkg-deps.cmake")

file(GLOB_RECURSE WORKER_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
)

file(GLOB_RECURSE WORKER_HEADERS
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)

find_path(BEXT_DI_INCLUDE_DIRS "boost/di.hpp")

set(DOTENV_SRC  "${CMAKE_CURRENT_SOURCE_DIR}/.env")
set(DOTENV_DEST "${CMAKE_CURRENT_BINARY_DIR}/.env")

option(ENABLE_DOTENV_COPY "Copy .env into the build dir (dev/deploy only)" ON)

if(ENABLE_DOTENV_COPY)
  if(EXISTS "${DOTENV_SRC}")
    configure_file("${DOTENV_SRC}" "${DOTENV_DEST}" COPYONLY)
    message(STATUS "Copied .env -> ${DOTENV_DEST}")
  else()
    message(STATUS "Skipping .env copy (not found at ${DOTENV_SRC})")
  endif()
endif()

add_executable(SkeletonizationWorker)

target_sources(SkeletonizationWorker
    PRIVATE
    ${WORKER_SOURCES}
    PRIVATE
    FILE_SET HEADERS
    BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
    FILES ${WORKER_HEADERS}
)

target_include_directories(SkeletonizationWorker PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include ${BEXT_DI_INCLUDE_DIRS} ${AWSSDK_INCLUDE_DIRS})

target_link_libraries(SkeletonizationWorker PRIVATE Skeletonization::Core Skeletonization::CoreCPU RapidJSON rapidjson hiredis::hiredis hiredis::hiredis_ssl libenvpp::libenvpp ${AWSSDK_LIBRARIES})

if(SKELETONIZATION_ENABLE_GPU)
    target_link_libraries(SkeletonizationWorker PRIVATE Skeletonization::CoreGPU)
endif()

if(SKELETONIZATION_ENABLE_MT)
    target_link_libraries(SkeletonizationWorker PRIVATE Skeletonization::CoreMT)
endif()
