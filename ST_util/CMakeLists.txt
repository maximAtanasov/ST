# This file is part of the "slavicTales" project.
# You may use, distribute or modify this code under the terms
# of the GNU General Public License version 2.
# See LICENCE.txt in the root directory of the project.
#
# Author: Maxim Atanasov
# E-mail: atanasovmaksim1@gmail.com

cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(ST_util)

set(CMAKE_CXX_STANDARD 14)

include_directories(${PROJECT_SOURCE_DIR}/include)
enable_testing()
find_package(GTest REQUIRED)

add_library(ST_util STATIC
        include/ST_util/debug_switch.hpp
        include/ST_util/test_util.hpp
        include/ST_util/math.hpp
        src/main/util.cpp
        src/main/math.cpp
        src/main/test_util.cpp
        src/main/string_util.cpp
        include/ST_util/string_util.hpp include/ST_util/bytell_hash_map.hpp include/ST_util/flat_hash_map.hpp include/ST_util/flat_hash_map.hpp)