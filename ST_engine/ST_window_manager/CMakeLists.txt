# This file is part of the "slavicTales" project.
# You may use, distribute or modify this code under the terms
# of the GNU General Public License version 2.
# See LICENCE.txt in the root directory of the project.
#
# Author: Maxim Atanasov
# E-mail: atanasovmaksim1@gmail.com

cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(ST_window_manager)

set(CMAKE_CXX_STANDARD 14)

include(GoogleTest)
enable_testing()

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../../cmake)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)


include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_IMAGE_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/src/main)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/../../ST_util/include)
include_directories(${PROJECT_SOURCE_DIR}/../ST_message_bus/include)
include_directories(${PROJECT_SOURCE_DIR}/../ST_task_manager/include)

add_library(ST_window_manager STATIC
        ${PROJECT_SOURCE_DIR}/include/window_manager.hpp
        ${PROJECT_SOURCE_DIR}/src/main/window_manager.cpp)

add_executable(window_manager_test
        ${PROJECT_SOURCE_DIR}/src/test/window_manager_tests.cpp)

target_link_libraries(window_manager_test
        gtest
        ST_window_manager
        ST_task_manager
        ST_message_bus
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARY})

#Run the tests on each build
add_test(NAME window_manager_test COMMAND window_manager_test WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src/test/test_resources)
add_custom_command(
        TARGET ST_window_manager
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target window_manager_test
)

if(DEFINED ENV{CIRCLECI} OR DEFINED ENV{APPVEYOR})
    add_custom_command(
            TARGET ST_window_manager
            POST_BUILD
            COMMAND ctest -VV
    )
endif()
