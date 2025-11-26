find_package(OpenCV CONFIG REQUIRED)

target_include_directories(SkeletonizationCore PUBLIC ${OpenCV_INCLUDE_DIRS})

target_link_libraries(SkeletonizationCore PUBLIC ${OpenCV_LIBS})