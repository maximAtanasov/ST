cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(ST_asset_pack)

set(CMAKE_CXX_STANDARD 11)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

include_directories(../ST_loaders/include)
include_directories(${PROJECT_SOURCE_DIR}/../libs/include)
link_directories(${PROJECT_SOURCE_DIR}/../libs/lib)

set(CMAKE_MODULE_PATH ../cmake)

if(UNIX AND NOT APPLE)
    set(SDL2_LIBRARY -lSDL2 -lpthread)
    set(SDL2_IMAGE_LIBRARY -lSDL2_image)
    set(SDL2_MIXER_LIBRARY -lSDL2_mixer)
endif()

add_executable(ST_asset_pack src/main.cpp)

target_link_libraries(ST_asset_pack ST_loaders dl ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_MIXER_LIBRARY})

add_dependencies(ST_asset_pack
        SDL2
        SDL2_image
        SDL2_mixer)