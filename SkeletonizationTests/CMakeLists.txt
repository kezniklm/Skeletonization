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
        "${CMAKE_SOURCE_DIR}/SkeletonizationCLI/src/commandline/parser.cpp"
        "${CMAKE_SOURCE_DIR}/SkeletonizationCLI/src/configuration/parser.cpp"
        "${CMAKE_SOURCE_DIR}/SkeletonizationCLI/src/configuration/loader.cpp"
        "${CMAKE_SOURCE_DIR}/SkeletonizationWorker/src/infrastructure/redis_uri.cpp"
        "${CMAKE_SOURCE_DIR}/SkeletonizationWorker/src/application/services/job_processor.cpp"
        "${CMAKE_SOURCE_DIR}/SkeletonizationWorker/src/infrastructure/aws_sdk_initializer.cpp"
        "${CMAKE_SOURCE_DIR}/SkeletonizationWorker/src/infrastructure/image_service.cpp"
        "${CMAKE_SOURCE_DIR}/SkeletonizationWorker/src/infrastructure/local_object_storage.cpp"
        "${CMAKE_SOURCE_DIR}/SkeletonizationWorker/src/infrastructure/platform/console_ctrl_handler.cpp"
        "${CMAKE_SOURCE_DIR}/SkeletonizationWorker/src/infrastructure/rapidjson_job_deserializer.cpp"
        "${CMAKE_SOURCE_DIR}/SkeletonizationWorker/src/infrastructure/redis.cpp"
        "${CMAKE_SOURCE_DIR}/SkeletonizationWorker/src/infrastructure/redis_adapter.cpp"
        "${CMAKE_SOURCE_DIR}/SkeletonizationWorker/src/infrastructure/redis_ssl.cpp"
        "${CMAKE_SOURCE_DIR}/SkeletonizationWorker/src/infrastructure/result_publisher.cpp"
        "${CMAKE_SOURCE_DIR}/SkeletonizationWorker/src/infrastructure/s3_object_storage.cpp"
        "${CMAKE_SOURCE_DIR}/SkeletonizationWorker/src/infrastructure/skeletonization_processor.cpp"
)

target_include_directories(SkeletonizationTests
    PRIVATE
        "${CMAKE_SOURCE_DIR}/SkeletonizationCLI/include"
        "${CMAKE_SOURCE_DIR}/SkeletonizationCore/include"
        "${CMAKE_SOURCE_DIR}/SkeletonizationWorker/include"
        ${AWSSDK_INCLUDE_DIRS}
)

target_link_libraries(SkeletonizationTests
    PRIVATE
        SkeletonizationCore
        SkeletonizationCoreCPU
        SkeletonizationCoreMT
        GTest::gtest_main
        CLI11::CLI11
        RapidJSON
        glog::glog
        libenvpp::libenvpp
        hiredis::hiredis
        hiredis::hiredis_ssl
        ${AWSSDK_LIBRARIES}
)

if(SKELETONIZATION_ENABLE_GPU)
    target_link_libraries(SkeletonizationTests
        PRIVATE
            SkeletonizationCoreGPU
    )
endif()

include(GoogleTest)

gtest_discover_tests(SkeletonizationTests)
set_property(TARGET SkeletonizationTests PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

