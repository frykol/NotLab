if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "Zbuduj projekt w osobnym katalogu, np. mkdir build && cd build && cmake ..")
endif()

cmake_minimum_required(VERSION 3.20)
project(NotLab)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR}/core)
include_directories(${CMAKE_SOURCE_DIR}/algorithms)
include_directories(${CMAKE_SOURCE_DIR}/equation_parser)

add_executable(NotLab src/main.cpp)

