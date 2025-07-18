cmake_minimum_required(VERSION 3.15)
project(morphlib VERSION 0.1.0 LANGUAGES CXX)

# Set default to C++17 (adjust as needed)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create the static library target
add_library(morphlib STATIC)

# Add source files
target_sources(morphlib
    PRIVATE
        src/Voice.cpp
        src/Synthesiser.cpp
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/morphlib/Voice.h>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/morphlib/Synthesiser.h>
)

# Add include directories for users of the library
target_include_directories(morphlib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# Optionally add compiler warnings
target_compile_options(morphlib PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)