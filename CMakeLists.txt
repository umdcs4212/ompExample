cmake_minimum_required(VERSION 3.22)

project(ompExample
  VERSION 1.0
  LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --------------------------------------------------------------------
# PNG and ZLIB
#
# This macro will attempt to locate the PNG include directories, link directories and libraries. The INCLUDE_DIRECTORIES
# macro adds the resulting include directories to the search path.
# --------------------------------------------------------------------
FIND_PACKAGE(PNG REQUIRED)
if(NOT PNG_FOUND)
  message(STATUS "Could not find the PNG Libraries!")
endif(NOT PNG_FOUND)

find_package(OpenMP)

add_executable(ompExample
  ompExample.cpp
)

target_include_directories(ompExample PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(ompExample PRIVATE PNG::PNG)
target_link_libraries(ompExample PRIVATE ZLIB::ZLIB)

if(OpenMP_CXX_FOUND)
  target_link_libraries(ompExample PRIVATE OpenMP::OpenMP_CXX)
endif()
