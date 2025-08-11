# cmake_minimum_required(VERSION 3.15)
# project(morphlib VERSION 0.1.0 LANGUAGES CXX)

# # Set default to C++17 (adjust as needed)
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # Create the static library target
# add_library(morphlib STATIC)

# # Add source files
# target_sources(morphlib
#     PRIVATE
#         src/Voice.cpp
#         src/Synthesizer.cpp
#     PUBLIC
#         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/morphlib/Voice.h>
#         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/morphlib/Synthesizer.h>
# )


# set(CMAKE_POSITION_INDEPENDENT_CODE ON) #necessary for GCC linking
# add_library(MTS_ESP_MORPH STATIC ${CMAKE_CURRENT_SOURCE_DIR}/external/MTS-ESP/Client/libMTSClient.cpp)

# # Add include directories for users of the library
# target_include_directories(morphlib
#     PUBLIC
#         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/>
# )


# # JUCE modules used in this lib
# target_link_libraries(morphlib
#     PRIVATE
#         juce::juce_audio_basics
#         juce::juce_dsp
#         MTS_ESP_MORPH
# )

# # Optionally add compiler warnings
# target_compile_options(morphlib PRIVATE
#     $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
#     $<$<CXX_COMPILER_ID:MSVC>:/W4>
# )

cmake_minimum_required(VERSION 3.15)
project(morphlib VERSION 0.1.0 LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Show target folders in IDEs and place these under "ThirdParty"
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# ------------------------------------------------------------------------------

# Static library target
add_library(morphlib STATIC)

# Sources/headers (headers are PRIVATE so they don't appear in dependents)
set(MORPHLIB_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Voice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Synthesizer.cpp
)
set(MORPHLIB_HDR
    ${CMAKE_CURRENT_SOURCE_DIR}/include/morphlib/Voice.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/morphlib/Synthesizer.h
)

target_sources(morphlib
    PRIVATE
        ${MORPHLIB_SRC}
        ${MORPHLIB_HDR}
)

# Nice IDE grouping inside the morphlib project
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src
             PREFIX "src"
             FILES ${MORPHLIB_SRC})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include
             PREFIX "include"
             FILES ${MORPHLIB_HDR})

set_target_properties(morphlib PROPERTIES FOLDER "ThirdParty")

# ------------------------------------------------------------------------------

# Position independent for GCC/Clang
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# MTS-ESP client (kept separate)
add_library(MTS_ESP_MORPH STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/external/MTS-ESP/Client/libMTSClient.cpp
)
set_target_properties(MTS_ESP_MORPH PROPERTIES FOLDER "ThirdParty")

# ------------------------------------------------------------------------------

# Include directories exposed to dependents
target_include_directories(morphlib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/>
)

# JUCE modules used in this lib
target_link_libraries(morphlib
    PRIVATE
        juce::juce_audio_basics
        juce::juce_dsp
        MTS_ESP_MORPH
)

# Warnings
target_compile_options(morphlib PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)
