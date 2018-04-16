# Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
# You may not use, distribute or modify this code.
# This code is proprietary and belongs to the "slavicTales"
# project. See LICENCE.txt in the root directory of the project.
#
# E-mail: atanasovmaksim1@gmail.com


cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(ST_asset_pack)

set(CMAKE_CXX_STANDARD 11)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

include_directories(../ST_loaders/include)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../cmake)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)

add_executable(ST_asset_pack src/main.cpp)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_IMAGE_INCLUDE_DIR})
include_directories(${SDL2_MIXER_INCLUDE_DIR})
target_link_libraries(ST_asset_pack ST_loaders ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_MIXER_LIBRARY})