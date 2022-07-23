# This file is part of the "ST" project.
# You may use, distribute or modify this code under the terms
# of the GNU General Public License version 2.
# See LICENCE.txt in the root directory of the project.
#
# Author: Maxim Atanasov
# E-mail: maxim.atanasov@protonmail.com

cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(ST_renderer)

set(CMAKE_CXX_STANDARD 20)

include_directories(${PROJECT_SOURCE_DIR}/include)
include(GoogleTest)
enable_testing()

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../../cmake)

find_package(SDL2 REQUIRED)
find_package(SDL2_IMAGE REQUIRED)
find_package(SDL2_MIXER REQUIRED)
find_package(SDL2_TTF REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_IMAGE_INCLUDE_DIR})
include_directories(${SDL2_MIXER_INCLUDE_DIR})
include_directories(${SDL2_TTF_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/../..ST_util/include)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src/main/renderer_sdl)
include_directories(${PROJECT_SOURCE_DIR}/../../ST_util/include)

add_library(ST_renderer STATIC
        ${PROJECT_SOURCE_DIR}/src/main/renderer_sdl/renderer_sdl.cpp
        ${PROJECT_SOURCE_DIR}/include/renderer_sdl.hpp
        ${PROJECT_SOURCE_DIR}/src/main/renderer_sdl/font_cache.cpp
        ${PROJECT_SOURCE_DIR}/src/main/renderer_sdl/font_cache.hpp
        ${PROJECT_SOURCE_DIR}/src/main/renderer_sdl/texture.hpp)

add_executable(renderer_sdl_test
        ${PROJECT_SOURCE_DIR}/include/renderer_sdl.hpp
        ${PROJECT_SOURCE_DIR}/src/main/renderer_sdl/font_cache.cpp
        ${PROJECT_SOURCE_DIR}/src/main/renderer_sdl/font_cache.hpp
        ${PROJECT_SOURCE_DIR}/src/test/renderer_sdl/renderer_sdl_tests.cpp
        ${PROJECT_SOURCE_DIR}/src/main/renderer_sdl/texture.hpp)

include_directories(renderer_sdl_test ../src/test ../ST_loaders/include)

target_link_libraries(renderer_sdl_test
        ST_loaders
        ST_util
        gtest
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
        ${SDL2_TTF_LIBRARY}
        ${SDL2_MIXER_LIBRARY})

set_target_properties(renderer_sdl_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/src/test/test_resources)
add_test(NAME renderer_sdl_test COMMAND renderer_sdl_test WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src/test/test_resources)

if(DEFINED ENV{CIRCLECI} OR DEFINED ENV{APPVEYOR})
    add_custom_command(
            TARGET ST_renderer
            POST_BUILD
            COMMAND renderer_sdl_test
			WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src/test/test_resources
    )
endif()