cmake_minimum_required(VERSION 3.30)
project(SkeletonizationTests)

include(${CMAKE_CURRENT_SOURCE_DIR}/vcpkg-deps.cmake)

file(GLOB_RECURSE TESTS_SOURCES CONFIGURE_DEPENDS tests/*.cpp)

file(GLOB_RECURSE TEST_MODULES CONFIGURE_DEPENDS *.ixx)

add_executable(SkeletonizationTests ${TESTS_SOURCES})

target_sources(SkeletonizationTests
    PUBLIC
    FILE_SET modules TYPE CXX_MODULES FILES
    ${TEST_MODULES}
)

target_link_libraries(SkeletonizationTests PRIVATE SkeletonizationCore GTest::gtest_main)

if(SKELETONIZATION_ENABLE_GPU)
    target_link_libraries(SkeletonizationTests PRIVATE SkeletonizationCoreGPU)
endif()

include(GoogleTest)

enable_testing()

gtest_discover_tests(SkeletonizationTests DISCOVERY_MODE POST_BUILD)
