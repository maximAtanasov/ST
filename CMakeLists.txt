# This file is part of the "slavicTales" project.
# You may use, distribute or modify this code under the terms
# of the GNU General Public License version 2.
# See LICENCE.txt in the root directory of the project.
#
# Author: Maxim Atanasov
# E-mail: atanasovmaksim1@gmail.com


cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(slavicTales)

add_subdirectory(ST_loaders)
add_subdirectory(ST_engine)
add_subdirectory(ST_asset_pack)
add_subdirectory(ST_util)

