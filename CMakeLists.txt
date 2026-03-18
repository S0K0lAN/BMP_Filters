cmake_minimum_required(VERSION 3.10.0)
project(BMP_Filters VERSION 0.0.1 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(BMP_Filters main.cpp BMPImageLib.cpp )

enable_testing()

add_executable(my_test tests.cpp)

add_test(NAME Tests_For_BMPFilters_Class COMMAND my_test)
