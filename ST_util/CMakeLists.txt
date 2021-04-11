# This file is part of the "ST" project.
# You may use, distribute or modify this code under the terms
# of the GNU General Public License version 2.
# See LICENCE.txt in the root directory of the project.
#
# Author: Maxim Atanasov
# E-mail: maxim.atanasov@protonmail.com

cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(ST_util)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../cmake)

find_package(SDL2 REQUIRED)
find_package(SDL2_IMAGE REQUIRED)
find_package(SDL2_MIXER REQUIRED)
find_package(SDL2_TTF REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_IMAGE_INCLUDE_DIR})
include_directories(${SDL2_MIXER_INCLUDE_DIR})
include_directories(${SDL2_TTF_INCLUDE_DIR})

include_directories(${PROJECT_SOURCE_DIR}/include)
include(GoogleTest)
enable_testing()

find_package(SDL2 REQUIRED)
find_package(SDL2_IMAGE REQUIRED)
find_package(SDL2_MIXER REQUIRED)
find_package(SDL2_TTF REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_IMAGE_INCLUDE_DIR})
include_directories(${SDL2_MIXER_INCLUDE_DIR})
include_directories(${SDL2_TTF_INCLUDE_DIR})

add_library(ST_util
        include/ST_util/test_util.hpp
        include/ST_util/VMem/VMem.cpp
        include/ST_util/VMem/VMem.h
        include/ST_util/math.hpp
        src/main/math.cpp
        src/main/test_util.cpp
        src/main/string_util.cpp
        include/ST_util/string_util.hpp
        include/ST_util/bytell_hash_map.hpp
        include/ST_util/flat_hash_map.hpp
        include/ST_util/flat_hash_map.hpp
        include/ST_util/pool_allocator_256.hpp
        include/ST_util/linear_frame_allocator_256.hpp)

add_executable(pool_allocator_256_test
        src/test/pool_allocator_256_tests.cpp
        include/ST_util/pool_allocator_256.hpp)

target_link_libraries(pool_allocator_256_test
        gtest)

set(RUN_ON_BUILD_TESTS
        pool_allocator_256_test)


#Run the tests on each build
foreach(loop_var ${RUN_ON_BUILD_TESTS})
    add_custom_command(
            TARGET ST_util
            POST_BUILD
            COMMAND ${loop_var}
    )
endforeach()
