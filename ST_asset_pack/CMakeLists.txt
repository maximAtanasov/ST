# This file is part of the "slavicTales" project.
# You may use, distribute or modify this code under the terms
# of the GNU General Public License version 2.
# See LICENCE.txt in the root directory of the project.
#
# Author: Maxim Atanasov
# E-mail: atanasovmaksim1@gmail.com


cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(ST_asset_pack)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../cmake)
set(CMAKE_CXX_STANDARD 14)

include_directories(../ST_loaders/include)
include_directories(../ST_util/include)
include(GoogleTest)
enable_testing()

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_executable(ST_asset_pack src/main/main.cpp src/main/main.hpp)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_IMAGE_INCLUDE_DIR})
include_directories(${SDL2_MIXER_INCLUDE_DIR})
include_directories(${SDL2_TTF_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/src/main)

target_link_libraries(ST_asset_pack
        ST_loaders
        ST_util
        gtest
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
        ${SDL2_MIXER_LIBRARY}
        ${SDL2_TTF_LIBRARY})

add_executable(ST_asset_pack_tests
        src/test/ST_asset_pack_tests.cpp
        src/main/main.hpp)

target_link_libraries(ST_asset_pack_tests
        ST_loaders
        ST_util
        gtest
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
        ${SDL2_MIXER_LIBRARY}
        ${SDL2_TTF_LIBRARY})

gtest_add_tests(TARGET ST_asset_pack_tests)
set_target_properties(ST_asset_pack_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/src/test/output)


# Run the tests on every build
add_custom_command(TARGET ST_asset_pack
        POST_BUILD
        COMMAND ST_asset_pack_tests)

set_target_properties(ST_asset_pack PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})
