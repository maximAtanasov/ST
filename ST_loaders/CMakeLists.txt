cmake_minimum_required(VERSION 3.9)
project(ST_loaders)

set(CMAKE_CXX_STANDARD 11)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/../libs/include)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

add_library(ST_loaders STATIC src/main/loaders.cpp include/ST_loaders/loaders.hpp)

add_dependencies(ST_loaders
        SDL2
        SDL2_image
        SDL2_mixer)