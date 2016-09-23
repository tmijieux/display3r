cmake_minimum_required(VERSION 2.8)

project(display3r CXX)
set(CMAKE_MODULE_PATH ${display3r_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})


if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()

set(Boost_USE_STATIC_LIBS ON)
add_definitions( -DBOOST_ALL_NO_LIB )



find_package(Sanitizers)
find_package(SDL2 REQUIRED)
find_package(Boost COMPONENTS regex program_options filesystem REQUIRED)
find_package(GLM)

include_directories(include)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${SDL2_INCLUDE_DIR})
include_directories(${GLM_INCLUDE_DIRS})

add_subdirectory(include)
add_subdirectory(src)

install(
  FILES config.ini
  DESTINATION bin
  )

install(
  DIRECTORY models textures
  DESTINATION bin
  )
