# This file is part of the "slavicTales" project.
# You may use, distribute or modify this code under the terms
# of the GNU General Public License version 2.
# See LICENCE.txt in the root directory of the project.
#
# Author: Maxim Atanasov
# E-mail: atanasovmaksim1@gmail.com

cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(ST_dev_console)

set(CMAKE_CXX_STANDARD 14)

include(GoogleTest)
enable_testing()

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../../cmake)


include_directories(${PROJECT_SOURCE_DIR}/src/main)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/../../ST_util/include)
include_directories(${PROJECT_SOURCE_DIR}/../ST_message_bus/include)
include_directories(${PROJECT_SOURCE_DIR}/../ST_input_manager/include)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_IMAGE_INCLUDE_DIR})
include_directories(${SDL2_MIXER_INCLUDE_DIR})
include_directories(${SDL2_TTF_INCLUDE_DIR})

add_library(ST_dev_console STATIC
        ${PROJECT_SOURCE_DIR}/src/main/console.cpp
        ${PROJECT_SOURCE_DIR}/src/main/console_log.hpp
        ${PROJECT_SOURCE_DIR}/include/console.hpp
        ${PROJECT_SOURCE_DIR}/src/main/console_log.cpp)

add_executable(dev_console_test
		${PROJECT_SOURCE_DIR}/src/main/console.cpp
        ${PROJECT_SOURCE_DIR}/src/main/console_log.hpp
        ${PROJECT_SOURCE_DIR}/include/console.hpp
        ${PROJECT_SOURCE_DIR}/src/main/console_log.cpp
        ${PROJECT_SOURCE_DIR}/src/test/console_tests.cpp)

target_link_libraries(dev_console_test
        gtest
        ST_message_bus)

gtest_add_tests(TARGET dev_console_test)

#Run the tests on each build
add_custom_command(
        TARGET ST_dev_console
        POST_BUILD
        COMMAND dev_console_test
)
