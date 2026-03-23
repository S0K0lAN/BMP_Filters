# CMakeLists for tests subdirectory

# This file is intended to be included via add_subdirectory from the top-level CMakeLists.
# It should not call project() again.

add_executable(${PROJECT_NAME}_tests
    tests.cpp
)

target_include_directories(${PROJECT_NAME}_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

target_link_libraries(${PROJECT_NAME}_tests
    PRIVATE
        ${PROJECT_NAME}_lib
)

add_test(NAME ${PROJECT_NAME}_tests COMMAND ${PROJECT_NAME}_tests)
