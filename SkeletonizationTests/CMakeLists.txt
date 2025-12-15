# CMakeList.txt : SkeletonizationTests

include("${CMAKE_CURRENT_SOURCE_DIR}/vcpkg-deps.cmake")

file(GLOB_RECURSE TESTS_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp"
)

file(GLOB_RECURSE TESTS_HEADERS
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)

add_executable(SkeletonizationTests)

target_sources(SkeletonizationTests
    PRIVATE
        ${TESTS_SOURCES}
        ${TESTS_HEADERS}
)

target_link_libraries(SkeletonizationTests
    PRIVATE
        SkeletonizationCoreCPU
        SkeletonizationCoreMT
        GTest::gtest_main
)

if(SKELETONIZATION_ENABLE_GPU)
    target_link_libraries(SkeletonizationTests
        PRIVATE
            SkeletonizationCoreGPU
    )
endif()

include(GoogleTest)

gtest_discover_tests(SkeletonizationTests)

