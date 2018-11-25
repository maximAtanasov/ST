# This file is part of the "slavicTales" project.
# You may use, distribute or modify this code under the terms
# of the GNU General Public License version 2.
# See LICENCE.txt in the root directory of the project.
#
# Author: Maxim Atanasov
# E-mail: atanasovmaksim1@gmail.com

cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(ST_message_bus)

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

add_library(ST_message_bus STATIC
        ${PROJECT_SOURCE_DIR}/src/main/message_allocator.cpp
        ${PROJECT_SOURCE_DIR}/src/main/message_allocator.hpp
        ${PROJECT_SOURCE_DIR}/src/main/message_bus.cpp
        ${PROJECT_SOURCE_DIR}/include/message_bus.hpp
        ${PROJECT_SOURCE_DIR}/src/main/subscriber.cpp
        ${PROJECT_SOURCE_DIR}/src/main/subscriber.hpp
        ${PROJECT_SOURCE_DIR}/src/main/message.hpp)

add_executable(message_test
        ${PROJECT_SOURCE_DIR}/src/test/message_tests.cpp)

target_link_libraries(message_test
        ST_util
        gtest
        ST_message_bus
        ${SDL2_LIBRARY})

add_executable(message_allocator_test
        ${PROJECT_SOURCE_DIR}/src/test/message_allocator_tests.cpp)

target_link_libraries(message_allocator_test
        ST_util
        gtest
        ST_message_bus
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
        ${SDL2_TTF_LIBRARY}
        ${SDL2_MIXER_LIBRARY})

add_executable(subscriber_test
        ${PROJECT_SOURCE_DIR}/src/test/subscriber_tests.cpp)

target_link_libraries(subscriber_test
        ST_util
        ST_message_bus
        gtest
        ${SDL2_LIBRARY})

add_executable(message_bus_test
        ${PROJECT_SOURCE_DIR}/src/test/message_bus_tests.cpp)

target_link_libraries(message_bus_test
        ST_util
        ST_message_bus
        gtest
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
        ${SDL2_TTF_LIBRARY}
        ${SDL2_MIXER_LIBRARY})


set(RUN_ON_BUILD_TESTS
        message_bus_test
        message_allocator_test
        subscriber_test
        message_test    )

gtest_add_tests(TARGET ${RUN_ON_BUILD_TESTS})

#Run the tests on each build
foreach(test ${RUN_ON_BUILD_TESTS})
    add_custom_command(
            TARGET ST_message_bus
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target ${test}
    )
endforeach()

add_custom_command(
        TARGET ST_message_bus
        POST_BUILD
        COMMAND ctest -VV
)
