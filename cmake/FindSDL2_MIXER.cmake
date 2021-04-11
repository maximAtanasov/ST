# Locate the SDL2_mixer library. This CMake module is a modified version
# of the original FindSDL_mixer.cmake file
# ###########################################################################
# Locate SDL_mixer library
# This module defines
# SDL2MIXER_LIBRARY, the name of the library to link against
# SDLMIXER_FOUND, if false, do not try to link to SDL
# SDL2MIXER_INCLUDE_DIR, where to find SDL/SDL.h
#
# $SDLDIR is an environment variable that would
# correspond to the ./configure --prefix=$SDLDIR
# used in building SDL.
#
# Created by Eric Wing. This was influenced by the FindSDL.cmake
# module, but with modifications to recognize OS X frameworks and
# additional Unix paths (FreeBSD, etc).

#=============================================================================
# Copyright 2005-2009 Kitware, Inc.
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distributed this file outside of CMake, substitute the full
#  License text for the above reference.)

FIND_PATH(SDL2_MIXER_INCLUDE_DIR SDL_mixer.h
        HINTS
        $ENV{SDL2_MIXER_DIR}
        $ENV{SDL2_DIR}
        PATH_SUFFIXES include
        PATHS
        ~/Library/Frameworks
        /Library/Frameworks
        /usr/local/include/SDL2
        /usr/include/SDL2
        /sw/include/SDL2 # Fink
        /opt/local/include/SDL2 # DarwinPorts
        /opt/csw/include/SDL2 # Blastwave
        /opt/include/SDL2
        ../external/SDL2/SDL2_mixer/include
        )


# Lookup the 64 bit libs on x64
IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
FIND_LIBRARY(SDL2_MIXER_LIBRARY
NAMES SDL2_mixer
HINTS
${SDL2}
$ENV{SDL2}
$ENV{SDL2_mixer}
PATH_SUFFIXES lib64 lib
lib/x64
x86_64-w64-mingw32/lib
PATHS
/sw
/opt/local
/opt/csw
/opt
../external/SDL2/SDL2_mixer/lib/x64
)
# On 32bit build find the 32bit libs
ELSE(CMAKE_SIZEOF_VOID_P EQUAL 8)
FIND_LIBRARY(SDL2_MIXER_LIBRARY
NAMES SDL2_mixer
HINTS
${SDL2}
$ENV{SDL2}
$ENV{SDL2_mixer}
PATH_SUFFIXES lib
lib/x86
i686-w64-mingw32/lib
PATHS
/sw
/opt/local
/opt/csw
/opt
../external/SDL2/SDL2_mixer/lib/x86
)
ENDIF(CMAKE_SIZEOF_VOID_P EQUAL 8)

SET(SDL2_MIXER_FOUND "NO")
IF(SDL2_MIXER_LIBRARY AND SDL2_MIXER_INCLUDE_DIR)
    SET(SDL2MIXER_FOUND "YES")
ENDIF(SDL2_MIXER_LIBRARY AND SDL2_MIXER_INCLUDE_DIR)

