# Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
# You may not use, distribute or modify this code.
# This code is proprietary and belongs to the "slavicTales"
# project. See LICENCE.txt in the root directory of the project.
#
# E-mail: atanasovmaksim1@gmail.com

cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(ST_loaders)

set(CMAKE_CXX_STANDARD 11)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(../ST_util/include)

add_library(ST_loaders STATIC
        src/main/loaders.cpp
        include/ST_loaders/loaders.hpp)