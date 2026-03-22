cmake_minimum_required(VERSION 3.10.0)

project(image_processor
    VERSION 0.0.1
    LANGUAGES C CXX
)

# -----------------------------------------------------------------------------
# Compiler settings
# -----------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable warnings and treat warnings as errors when requested
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# -----------------------------------------------------------------------------
# Source organization
# -----------------------------------------------------------------------------
set(image_processor_SOURCES
    src/main.cpp
    src/BMPImageLib.cpp
    src/Filter.cpp
    src/ActionManager.cpp
)

set(image_processor_HEADERS
    src/BMPImageLib.h
    src/Filter.h
    src/ActionManager.h
    src/pack_defines.h
)

# Library target for sources
add_library(${PROJECT_NAME}_lib STATIC
    src/BMPImageLib.cpp
    src/Filter.cpp
    src/ActionManager.cpp
)

target_include_directories(${PROJECT_NAME}_lib
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Main executable
add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_NAME}_lib
)

# -----------------------------------------------------------------------------
# Tests
# -----------------------------------------------------------------------------
option(BUILD_TESTS "Build unit tests" ON)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
