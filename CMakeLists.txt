# CMakeList.txt : Global project for Skeletonization

cmake_minimum_required (VERSION 3.30)
project (Skeletonization)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141 AND USE_SANITIZER EQUAL OFF)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Set IS_64BIT flag
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(IS_64BIT TRUE)
else()
    set(IS_64BIT FALSE)
endif()

# Set USE_SANITIZER to off by default
option(USE_SANITIZER "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)

# Enable sanitizers only in Debug builds
if(USE_SANITIZER AND CMAKE_BUILD_TYPE STREQUAL "Debug")

    # Enable sanitizers
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        message(STATUS "Enabling AddressSanitizer and UBSan for Clang/GCC")

        add_compile_options(-fsanitize=address,undefined)
        add_link_options(-fsanitize=address,undefined)

        if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            add_compile_options(-fno-omit-frame-pointer)
        endif()
    elseif(MSVC AND IS_64BIT)
        message(STATUS "Enabling AddressSanitizer and UBSan for MSVC x64")

        add_compile_options(/fsanitize=address /fsanitize=undefined)
        add_link_options(/INCREMENTAL:NO /fsanitize=address /fsanitize=undefined)
    endif()
endif()

# Enable performance optimizations
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Enabling Release optimizations for performance")

    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
         add_compile_options(-O3 -march=native)
    elseif (MSVC)
        add_compile_options(/arch:AVX2)
    endif()
endif()

# Set C++ standards
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(SkeletonizationCore)
add_subdirectory(SkeletonizationCoreGPU)
add_subdirectory(SkeletonizationPython)
add_subdirectory(SkeletonizationCLI)
