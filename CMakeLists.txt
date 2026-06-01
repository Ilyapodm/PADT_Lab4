cmake_minimum_required(VERSION 3.20)

project(PADT_Lab4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_subdirectory(include)
add_subdirectory(src)

enable_testing()
add_subdirectory(tests)