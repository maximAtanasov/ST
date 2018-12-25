# This file is part of the "slavicTales" project.
# You may use, distribute or modify this code under the terms
# of the GNU General Public License version 2.
# See LICENCE.txt in the root directory of the project.
#
# Author: Maxim Atanasov
# E-mail: atanasovmaksim1@gmail.com

cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(ST_task_manager)

set(CMAKE_CXX_STANDARD 14)

include(GoogleTest)
enable_testing()

include_directories(${PROJECT_SOURCE_DIR}/src/main)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/../../ST_util/include)
include_directories(${PROJECT_SOURCE_DIR}/../ST_message_bus/include)

add_library(ST_task_manager STATIC
        ${PROJECT_SOURCE_DIR}/src/main/task.hpp
        ${PROJECT_SOURCE_DIR}/src/main/task_manager.cpp
        ${PROJECT_SOURCE_DIR}/src/main/semaphore.hpp
        ${PROJECT_SOURCE_DIR}/include/task_manager.hpp
        ${PROJECT_SOURCE_DIR}/src/main/task_allocator.cpp
        ${PROJECT_SOURCE_DIR}/src/main/task_allocator.hpp)

add_executable(task_manager_test
		${PROJECT_SOURCE_DIR}/src/main/task.hpp
        ${PROJECT_SOURCE_DIR}/src/main/task_manager.cpp
        ${PROJECT_SOURCE_DIR}/src/main/semaphore.hpp
        ${PROJECT_SOURCE_DIR}/include/task_manager.hpp
        ${PROJECT_SOURCE_DIR}/src/main/task_allocator.cpp
        ${PROJECT_SOURCE_DIR}/src/main/task_allocator.hpp
        ${PROJECT_SOURCE_DIR}/src/test/task_manager_tests.cpp)

target_link_libraries(task_manager_test
        gtest
        ST_message_bus)

#Run the tests on each build
add_test(NAME task_manager_test COMMAND task_manager_test)
add_custom_command(
        TARGET ST_task_manager
        POST_BUILD
        COMMAND task_manager_test
)
