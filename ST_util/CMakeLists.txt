# Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
# You may not use, distribute or modify this code.
# This code is proprietary and belongs to the "slavicTales"
# project. See LICENCE.txt in the root directory of the project.
#
# E-mail: atanasovmaksim1@gmail.com

cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(ST_util)

set(CMAKE_CXX_STANDARD 11)

if(UNIX AND NOT APPLE)
    set(CMAKE_CXX_FLAGS_DEBUG  "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -Wextra -flto")
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)

enable_testing()

find_package(GTest REQUIRED)

add_library(ST_util STATIC include/ST_util/map.hpp
        src/main/math.cpp
        include/ST_util/math.hpp)

add_executable(map_test
        src/test/map_test.cpp)

target_link_libraries(map_test
        -pthread
        ${GTEST_LIBRARIES})

gtest_add_tests(TARGET map_test)

#Run all our tests on every build
set(ALL_TESTS map_test)

add_dependencies(ST_util ${ALL_TESTS})

add_custom_command(TARGET ST_util
        POST_BUILD
        COMMAND ${ALL_TESTS})
