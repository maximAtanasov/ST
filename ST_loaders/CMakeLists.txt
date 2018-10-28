# This file is part of the "slavicTales" project.
# You may use, distribute or modify this code under the terms
# of the GNU General Public License version 2.
# See LICENCE.txt in the root directory of the project.
#
# Author: Maxim Atanasov
# E-mail: atanasovmaksim1@gmail.com

cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(ST_loaders)
set(CMAKE_CXX_STANDARD 14)
include(GoogleTest)
enable_testing()
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../cmake)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(../ST_util/include)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_IMAGE_INCLUDE_DIR})
include_directories(${SDL2_MIXER_INCLUDE_DIR})
include_directories(${SDL2_TTF_INCLUDE_DIR})

add_library(ST_loaders STATIC
        src/main/loaders.cpp
        include/ST_loaders/loaders.hpp)

add_executable(loaders_test
        src/test/loaders_test.cpp
        src/main/loaders.cpp
        include/ST_loaders/loaders.hpp)

target_link_libraries(loaders_test
        ST_util
        gtest
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
        ${SDL2_TTF_LIBRARY}
        ${SDL2_MIXER_LIBRARY})

set_target_properties(loaders_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/src/test/test_resources)

gtest_add_tests(TARGET loaders_test)

add_dependencies(ST_loaders loaders_test)

add_custom_command(
        TARGET ST_loaders
        POST_BUILD
        COMMAND loaders_test
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src/test/test_resources
)