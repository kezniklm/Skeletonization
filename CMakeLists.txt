# CMakeList.txt : Global project for Skeletonization

cmake_minimum_required(VERSION 3.30)

project(Skeletonization)

# Enable Hot Reload for MSVC compilers if supported.
if(POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Set IS_64BIT flag
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(IS_64BIT TRUE)
else()
    set(IS_64BIT FALSE)
endif()

# Enable CUDA debugging
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Enabling CUDA debugging")

    if(CMAKE_CUDA_COMPILER)
        add_compile_options($<$<COMPILE_LANGUAGE:CUDA>:-G>)
        add_compile_options($<$<COMPILE_LANGUAGE:CUDA>:-g>)
    endif()
endif()

# Enable performance optimizations
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Enabling Release optimizations for performance")

    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(
            $<$<COMPILE_LANGUAGE:CXX>:-O3>
            $<$<COMPILE_LANGUAGE:CXX>:-march=native>
            $<$<COMPILE_LANGUAGE:CXX>:-funroll-loops>
            $<$<COMPILE_LANGUAGE:CXX>:-Wpedantic>
        )
    elseif(MSVC)
        add_compile_options(
            $<$<COMPILE_LANGUAGE:CXX>:/O2>
            $<$<COMPILE_LANGUAGE:CXX>:/Ot>
            $<$<COMPILE_LANGUAGE:CXX>:/GL>
            $<$<COMPILE_LANGUAGE:CXX>:/arch:AVX2>
            $<$<COMPILE_LANGUAGE:CXX>:/W4>
            $<$<COMPILE_LANGUAGE:CXX>:/WX>
            $<$<COMPILE_LANGUAGE:CXX>:/permissive->
            $<$<COMPILE_LANGUAGE:CXX>:/Zc:__cplusplus>
            $<$<COMPILE_LANGUAGE:CXX>:/Zc:alignedNew>
            $<$<COMPILE_LANGUAGE:CXX>:/Zc:auto>
            $<$<COMPILE_LANGUAGE:CXX>:/Zc:char8_t>
            $<$<COMPILE_LANGUAGE:CXX>:/Zc:externConstexpr>
            $<$<COMPILE_LANGUAGE:CXX>:/Zc:forScope>
            $<$<COMPILE_LANGUAGE:CXX>:/Zc:gotoScope>
            $<$<COMPILE_LANGUAGE:CXX>:/Zc:hiddenFriend>
            $<$<COMPILE_LANGUAGE:CXX>:/Zc:implicitNoexcept>
            $<$<COMPILE_LANGUAGE:CXX>:/Zc:inline>
            $<$<COMPILE_LANGUAGE:CXX>:/Zc:noexceptTypes>
            $<$<COMPILE_LANGUAGE:CXX>:/Zc:preprocessor>
            $<$<COMPILE_LANGUAGE:CXX>:/Zc:referenceBinding>
            $<$<COMPILE_LANGUAGE:CXX>:/Zc:rvalueCast>
            $<$<COMPILE_LANGUAGE:CXX>:/Zc:sizedDealloc>
            $<$<COMPILE_LANGUAGE:CXX>:/Zc:strictStrings>
            $<$<COMPILE_LANGUAGE:CXX>:/Zc:ternary>
            $<$<COMPILE_LANGUAGE:CXX>:/Zc:throwingNew>
            $<$<COMPILE_LANGUAGE:CXX>:/Zc:tlsGuards>
            $<$<COMPILE_LANGUAGE:CXX>:/Zc:trigraphs->
            $<$<COMPILE_LANGUAGE:CXX>:/volatile:iso>
        )
    endif()

    if(CMAKE_CUDA_COMPILER)
        add_compile_options(
            $<$<COMPILE_LANGUAGE:CUDA>:-O3>
            $<$<COMPILE_LANGUAGE:CUDA>:--restrict>
            $<$<COMPILE_LANGUAGE:CUDA>:--ptxas-options=-O3>
        )
    endif()
endif()

# Set C++ standards
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_definitions(SKELETONIZATION_WITH_GPU=$<BOOL:${SKELETONIZATION_ENABLE_GPU}>)

add_subdirectory(SkeletonizationCore)
add_subdirectory(SkeletonizationCoreCPU)
add_subdirectory(SkeletonizationCoreMT)

if(SKELETONIZATION_ENABLE_GPU)
    find_package(CUDAToolkit REQUIRED)

    message(STATUS "CUDA preset requested and CUDA found: enabling GPU support")
    add_compile_definitions(SKELETONIZATION_WITH_GPU=1)
    add_subdirectory(SkeletonizationCoreGPU)
else()
    message(STATUS "CUDA preset not requested: disabling GPU support")
    add_compile_definitions(SKELETONIZATION_WITH_GPU=0)
endif()

add_subdirectory(SkeletonizationPython)
add_subdirectory(SkeletonizationVisualizer)
add_subdirectory(SkeletonizationTests)
#add_subdirectory(SkeletonizationWorker)
add_subdirectory(SkeletonizationCLI)
