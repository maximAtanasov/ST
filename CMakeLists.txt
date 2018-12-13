# This file is part of the "slavicTales" project.
# You may use, distribute or modify this code under the terms
# of the GNU General Public License version 2.
# See LICENCE.txt in the root directory of the project.
#
# Author: Maxim Atanasov
# E-mail: atanasovmaksim1@gmail.com


cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(slavicTales)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    set(CMAKE_CXX_FLAGS_RELEASE "/Ox /MD")
    set(CMAKE_CXX_FLAGS_DEBUG "/MDd")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE /STACK:3000000")
endif()

if($ENV{SDL_VIDEODRIVER})
    add_definitions(-DSDL_VIDEODRIVER=$ENV{SDL_VIDEODRIVER})
endif()

add_subdirectory(ST_loaders)
add_subdirectory(ST_engine)
add_subdirectory(ST_asset_pack)
add_subdirectory(ST_util)
add_subdirectory(external/googletest)
add_subdirectory(external/lua)

