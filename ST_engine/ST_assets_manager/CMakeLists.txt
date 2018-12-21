# This file is part of the "slavicTales" project.
# You may use, distribute or modify this code under the terms
# of the GNU General Public License version 2.
# See LICENCE.txt in the root directory of the project.
#
# Author: Maxim Atanasov
# E-mail: atanasovmaksim1@gmail.com

cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(ST_assets_manager)

set(CMAKE_CXX_STANDARD 14)

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
include_directories(${PROJECT_SOURCE_DIR}/src/main)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/../../ST_util/include)
include_directories(${PROJECT_SOURCE_DIR}/../../ST_loaders/include)
include_directories(${PROJECT_SOURCE_DIR}/../ST_message_bus/include)
include_directories(${PROJECT_SOURCE_DIR}/../ST_task_manager/include)


add_library(ST_assets_manager STATIC
        ${PROJECT_SOURCE_DIR}/include/assets_manager.hpp
        ${PROJECT_SOURCE_DIR}/src/main/assets_manager.cpp
        ${PROJECT_SOURCE_DIR}/include/assets.hpp)

add_executable(assets_manager_test
        ${PROJECT_SOURCE_DIR}/src/test/assets_manager_tests.cpp)

target_link_libraries(assets_manager_test
        gtest
        ST_loaders
        ST_assets_manager
        ST_task_manager
        ST_message_bus
        ST_util
        ST_task_manager
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
        ${SDL2_TTF_LIBRARY}
        ${SDL2_MIXER_LIBRARY}
        )

#Run the tests on each build
set_target_properties(assets_manager_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/src/test/test_resources)
add_test(NAME assets_manager_test COMMAND assets_manager_test WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src/test/test_resources)
add_custom_command(
        TARGET ST_assets_manager
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target assets_manager_test
)
add_custom_command(
        TARGET ST_assets_manager
        POST_BUILD
        COMMAND ctest -VV
)
