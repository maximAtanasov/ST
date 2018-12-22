# This file is part of the "slavicTales" project.
# You may use, distribute or modify this code under the terms
# of the GNU General Public License version 2.
# See LICENCE.txt in the root directory of the project.
#
# Author: Maxim Atanasov
# E-mail: atanasovmaksim1@gmail.com

cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(ST_input_manager)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../../cmake)

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/src/main)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/../../ST_util/include)
include_directories(${PROJECT_SOURCE_DIR}/../ST_message_bus/include)
include_directories(${PROJECT_SOURCE_DIR}/../ST_task_manager/include)


add_library(ST_input_manager STATIC
        ${PROJECT_SOURCE_DIR}/include/key_definitions.hpp
        ${PROJECT_SOURCE_DIR}/include/input_manager.hpp
        ${PROJECT_SOURCE_DIR}/src/main/input_manager.cpp)