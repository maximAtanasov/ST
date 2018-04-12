cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(ST_loaders)

set(CMAKE_CXX_STANDARD 11)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(ST_loaders STATIC
        src/main/loaders.cpp
        include/ST_loaders/loaders.hpp)