cmake_minimum_required(VERSION 2.8)

project(display3r CXX)
set(CMAKE_MODULE_PATH ${display3r_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

find_package(Sanitizers)
find_package(SDL2 REQUIRED)


set(Boost_USE_STATIC_LIBS ON)
add_definitions( -DBOOST_ALL_NO_LIB )
find_package(Boost COMPONENTS regex program_options filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

include_directories(include ${SDL2_INCLUDE_DIR})
add_subdirectory(src)

install(
  FILES config.ini
  DESTINATION bin
  )
