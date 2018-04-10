cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(slavicTales-master)

set(FREETYPE_VER "2.4.8")

message(${CMAKE_INSTALL_PREFIX})

set(LIBS_DIR ${PROJECT_SOURCE_DIR}/libs)

include(ExternalProject)
externalproject_add(SDL2
        URL https://hg.libsdl.org/SDL/archive/tip.zip
        DOWNLOAD_NAME SDL2.zip
        PREFIX ${LIBS_DIR}
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${LIBS_DIR}
        )

ExternalProject_Add(glew
        GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
       GIT_TAG master
       PREFIX ${LIBS_DIR}
       CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${LIBS_DIR}
)

ExternalProject_Add(gtest
        GIT_REPOSITORY https://github.com/google/googletest.git
        PREFIX ${LIBS_DIR}
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${LIBS_DIR}
        )

ExternalProject_Get_Property(SDL2 SOURCE_DIR)
ExternalProject_Get_Property(SDL2 BINARY_DIR)

set(SDL2_SRC ${SOURCE_DIR})
set(SDL2_BIN ${BINARY_DIR})

externalproject_add(lua5.3
        GIT_REPOSITORY https://github.com/LuaDist/lua.git
        GIT_TAG lua-5.3
        PREFIX ${LIBS_DIR}
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${LIBS_DIR}
        )

externalproject_add(zlib
        URL http://zlib.net/zlib-1.2.11.tar.gz
        PREFIX ${LIBS_DIR}
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${LIBS_DIR}
        )


externalproject_add(libpng
        URL ftp://ftp-osl.osuosl.org/pub/libpng/src/libpng16/libpng-1.6.34.tar.gz
        DEPENDS zlib
        PREFIX ${LIBS_DIR}
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${LIBS_DIR}
        )

externalproject_add(SDL2_image
        URL https://hg.libsdl.org/SDL_image/archive/tip.zip
        DOWNLOAD_NAME SDL2_IMAGE.zip
        PREFIX ${LIBS_DIR}
        DEPENDS SDL2 libpng
        CONFIGURE_COMMAND CFLAGS=-I${LIBS_DIR}/include LDFLAGS=-L${LIBS_DIR}/lib <SOURCE_DIR>/configure --prefix=${LIBS_DIR} --enable-shared=yes
        BUILD_COMMAND ${MAKE_EXE}
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${LIBS_DIR}
        )

externalproject_add(libogg
        URL http://downloads.xiph.org/releases/ogg/libogg-1.3.3.zip
        PREFIX ${LIBS_DIR}
        CONFIGURE_COMMAND CFLAGS=-I${LIBS_DIR}/include LDFLAGS=-L${LIBS_DIR}/lib <SOURCE_DIR>/configure --prefix=${LIBS_DIR} --enable-shared=yes
        BUILD_COMMAND ${MAKE_EXE}
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${LIBS_DIR}
        )

externalproject_add(libvorbis
        URL http://downloads.xiph.org/releases/vorbis/libvorbis-1.3.6.zip
        PREFIX ${LIBS_DIR}
        CONFIGURE_COMMAND CFLAGS=-I${LIBS_DIR}/include LDFLAGS=-L${LIBS_DIR}/lib <SOURCE_DIR>/configure --prefix=${LIBS_DIR} --enable-shared=yes
        BUILD_COMMAND ${MAKE_EXE}
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${LIBS_DIR}
        )

externalproject_add(SDL2_mixer
        URL https://hg.libsdl.org/SDL_mixer/archive/tip.zip
        DOWNLOAD_NAME SDL2_MIXER.zip
        PREFIX ${LIBS_DIR}
        DEPENDS SDL2 libogg libvorbis
        CONFIGURE_COMMAND CFLAGS=-I${LIBS_DIR}/include LDFLAGS=-L${LIBS_DIR}/lib <SOURCE_DIR>/configure --prefix=${LIBS_DIR} --enable-shared=yes
        BUILD_COMMAND ${MAKE_EXE}
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${LIBS_DIR}
        )

# Freetype library
externalproject_add(freetype
        URL https://download.savannah.gnu.org/releases/freetype/freetype-${FREETYPE_VER}.tar.gz
        PREFIX ${LIBS_DIR}
        BUILD_COMMAND ${MAKE_EXE}
        CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=${LIBS_DIR} --enable-shared=yes
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${LIBS_DIR}
        )


externalproject_add(SDL2_ttf
        URL https://hg.libsdl.org/SDL_ttf/archive/tip.zip
        DOWNLOAD_NAME SDL2_TTF.zip
        PREFIX ${LIBS_DIR}
        DEPENDS SDL2 freetype
        CONFIGURE_COMMAND CFLAGS=-I${LIBS_DIR}/include LDFLAGS=-L${LIBS_DIR}/lib <SOURCE_DIR>/configure FREETYPE_CONFIG=${LIBS_DIR}/bin/freetype-config --prefix=${LIBS_DIR} --enable-shared=yes
        BUILD_COMMAND ${MAKE_EXE}
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${LIBS_DIR}
        )

add_subdirectory(ST_loaders)
add_subdirectory(ST_engine)
add_subdirectory(ST_asset_pack)

