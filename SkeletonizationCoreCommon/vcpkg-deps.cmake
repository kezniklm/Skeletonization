find_package(OpenCV CONFIG REQUIRED)

target_include_directories(SkeletonizationCoreCommon PUBLIC ${OpenCV_INCLUDE_DIRS})

target_link_libraries(SkeletonizationCoreCommon PUBLIC ${OpenCV_LIBS})