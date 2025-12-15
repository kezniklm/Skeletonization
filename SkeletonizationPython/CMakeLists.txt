# CMakeList.txt : SkeletonizationPython

include("${CMAKE_CURRENT_SOURCE_DIR}/vcpkg-deps.cmake")

file(GLOB_RECURSE PYTHON_WRAPPER_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

file(GLOB_RECURSE PYTHON_WRAPPER_HEADERS
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)

pybind11_add_module(SkeletonizationPython
    ${PYTHON_WRAPPER_SOURCES}
    ${PYTHON_WRAPPER_HEADERS}
)

set_target_properties(SkeletonizationPython PROPERTIES OUTPUT_NAME "skeletonization")

target_include_directories(SkeletonizationPython
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(SkeletonizationPython
    PRIVATE
    Skeletonization::Core
    Skeletonization::CoreCPU
    Skeletonization::CoreMT
)

if(SKELETONIZATION_ENABLE_GPU)
    target_link_libraries(SkeletonizationPython
        PRIVATE
        Skeletonization::CoreGPU
    )
endif()
