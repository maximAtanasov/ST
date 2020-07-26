# This file is part of the "ST" project.
# You may use, distribute or modify this code under the terms
# of the GNU General Public License version 2.
# See LICENCE.txt in the root directory of the project.
#
# Author: Maxim Atanasov
# E-mail: maxim.atanasov@protonmail.com

cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(ST_task_manager)

set(CMAKE_CXX_STANDARD 20)

include(GoogleTest)
enable_testing()

include_directories(${PROJECT_SOURCE_DIR}/src/main)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/../../ST_util/include)

add_library(ST_task_manager STATIC
        ${PROJECT_SOURCE_DIR}/src/main/task.hpp
        ${PROJECT_SOURCE_DIR}/src/main/task_manager.cpp
        ${PROJECT_SOURCE_DIR}/src/main/semaphore.hpp
        ${PROJECT_SOURCE_DIR}/include/task_manager.hpp src/main/task.cpp)

add_executable(task_manager_test
		${PROJECT_SOURCE_DIR}/src/main/task.hpp
        ${PROJECT_SOURCE_DIR}/src/main/task_manager.cpp
        ${PROJECT_SOURCE_DIR}/src/main/semaphore.hpp
        ${PROJECT_SOURCE_DIR}/include/task_manager.hpp
        ${PROJECT_SOURCE_DIR}/src/test/task_manager_tests.cpp src/main/task.cpp)

target_link_libraries(task_manager_test
        gtest)

#Run the tests on each build
add_test(NAME task_manager_test COMMAND task_manager_test)
add_custom_command(
        TARGET ST_task_manager
        POST_BUILD
        COMMAND task_manager_test
)
