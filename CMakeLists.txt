cmake_minimum_required(VERSION 2.8)

project(display3r CXX)
set(CMAKE_MODULE_PATH ${display3r_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

find_package(Sanitizers)
find_package(SDL2 REQUIRED)

include_directories(include ${SDL2_INCLUDE_DIR})
add_subdirectory(src)

