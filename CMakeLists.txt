project(pevtx CXX)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "-std=c++11 -g")

include_directories(${CMAKE_SOURCE_DIR})

aux_source_directory(pevtx SOURCE)
add_library(${PROJECT_NAME} ${SOURCE})

add_subdirectory(example)
