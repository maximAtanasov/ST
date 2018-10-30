# This file is part of the "slavicTales" project.
# You may use, distribute or modify this code under the terms
# of the GNU General Public License version 2.
# See LICENCE.txt in the root directory of the project.
#
# Author: Maxim Atanasov
# E-mail: atanasovmaksim1@gmail.com

cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(ST_util)

set(CMAKE_CXX_STANDARD 14)

include_directories(${PROJECT_SOURCE_DIR}/include)
include(GoogleTest)
enable_testing()

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../../cmake)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_IMAGE_INCLUDE_DIR})
include_directories(${SDL2_MIXER_INCLUDE_DIR})
include_directories(${SDL2_TTF_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/../../ST_util/include)

add_library(ST_renderer STATIC
        ${PROJECT_SOURCE_DIR}/src/main/renderer_sdl/renderer_sdl.cpp
        ${PROJECT_SOURCE_DIR}/include/renderer_sdl.hpp
        ${PROJECT_SOURCE_DIR}/src/main/renderer_sdl/font_cache.cpp
        ${PROJECT_SOURCE_DIR}/src/main/renderer_sdl/font_cache.hpp
        )

add_executable(renderer_sdl_test
        ${PROJECT_SOURCE_DIR}/src/main/renderer_sdl/renderer_sdl.cpp
        ${PROJECT_SOURCE_DIR}/include/renderer_sdl.hpp
        ${PROJECT_SOURCE_DIR}/src/test/renderer_sdl/renderer_sdl_tests.cpp
        ${PROJECT_SOURCE_DIR}/src/main/renderer_sdl/font_cache.cpp
        ${PROJECT_SOURCE_DIR}/src/main/renderer_sdl/font_cache.hpp)

include_directories(renderer_sdl_test ../ST_engine/src/test ../ST_loaders/include)

target_link_libraries(renderer_sdl_test
        ST_loaders
        ST_util
        gtest
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
        ${SDL2_TTF_LIBRARY}
        ${SDL2_MIXER_LIBRARY})