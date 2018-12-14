# This file is part of the "slavicTales" project.
# You may use, distribute or modify this code under the terms
# of the GNU General Public License version 2.
# See LICENCE.txt in the root directory of the project.
#
# Author: Maxim Atanasov
# E-mail: atanasovmaksim1@gmail.com

cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(ST_engine)

set(CMAKE_CXX_STANDARD 14)

include(GoogleTest)
enable_testing()

add_subdirectory(ST_renderer)
add_subdirectory(ST_message_bus)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../cmake)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(../ST_loaders/include)
include_directories(../ST_util/include)
include_directories(../ST_engine/src/main)
include_directories(../ST_engine/ST_renderer/include)
include_directories(../ST_engine/ST_message_bus/include)
include_directories(../external/googletest/googletest/include)
include_directories(../external/googletest/googlemock/include)
include_directories(../external/lua/lua-5.3.0/src)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_IMAGE_INCLUDE_DIR})
include_directories(${SDL2_MIXER_INCLUDE_DIR})
include_directories(${SDL2_TTF_INCLUDE_DIR})

add_executable(ST_engine
        src/main/assets_manager/assets.hpp
        src/main/assets_manager/assets_manager.cpp
        src/main/assets_manager/assets_manager.hpp
        src/main/audio_manager/audio_manager.cpp
        src/main/audio_manager/audio_manager.hpp
        src/main/game_manager/level/camera.hpp
        src/main/main/message_types.hpp
        src/main/console/console.cpp
        src/main/console/console.hpp
        src/main/defs.hpp
        src/main/display_manager/display_manager.cpp
        src/main/display_manager/display_manager.hpp
        src/main/drawing_manager/drawing_manager.cpp
        src/main/drawing_manager/drawing_manager.hpp
        src/main/game_manager/level/entity.hpp
        src/main/game_manager/game_manager.cpp
        src/main/game_manager/game_manager.hpp
        src/main/input_manager/input_manager.cpp
        src/main/input_manager/input_manager.hpp
        src/main/game_manager/level/level.cpp
        src/main/game_manager/level/level.hpp
        src/main/game_manager/level/light.cpp
        src/main/game_manager/level/light.hpp
        src/main/main/main.cpp
        src/main/main/main.hpp
        src/main/physics_manager/physics_manager.cpp
        src/main/physics_manager/physics_manager.hpp
        src/main/task_manager/task.hpp
        src/main/task_manager/task_manager.cpp
        src/main/task_manager/task_manager.hpp
        src/main/game_manager/level/text.hpp
        src/main/main/timer.cpp
        src/main/main/timer.hpp
        src/main/main/fps.cpp
        src/main/main/fps.hpp
        src/main/game_manager/lua_backend/lua_backend.cpp
        src/main/game_manager/lua_backend/lua_backend.hpp
        src/main/input_manager/key_definitions.hpp
        src/main/console/console_log.cpp
        src/main/console/console_log.hpp
        src/main/console/log.hpp
        src/main/game_manager/level/text.cpp
        src/main/task_manager/task_allocator.cpp
        src/main/task_manager/task_allocator.hpp)

target_link_libraries(ST_engine
        ST_util
        ST_loaders
        ST_renderer
        ST_message_bus
        lua
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
        ${SDL2_TTF_LIBRARY}
        ${SDL2_MIXER_LIBRARY}
       )

add_executable(asset_manager_test
        src/main/assets_manager/assets.hpp
        src/main/assets_manager/assets_manager.cpp
        src/main/assets_manager/assets_manager.hpp
        src/main/defs.hpp
        src/main/task_manager/task.hpp
        src/main/task_manager/task_manager.cpp
        src/main/task_manager/task_manager.hpp
        src/main/console/console_log.cpp
        src/main/console/console_log.hpp
        src/main/console/log.hpp
        src/test/assets_manager/assets_manager_tests.cpp
        src/main/task_manager/task_allocator.cpp
        src/main/task_manager/task_allocator.hpp)

include_directories(asset_manager_test ../ST_engine/src/test)


target_link_libraries(asset_manager_test
        ST_loaders
        ST_util
        ST_message_bus
        gtest
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
        ${SDL2_TTF_LIBRARY}
        ${SDL2_MIXER_LIBRARY})


add_executable(audio_manager_test
        src/main/assets_manager/assets.hpp
        src/main/audio_manager/audio_manager.cpp
        src/main/audio_manager/audio_manager.hpp
        src/main/defs.hpp
        src/main/task_manager/task.hpp
        src/main/task_manager/task_manager.cpp
        src/main/task_manager/task_manager.hpp
        src/main/console/console_log.cpp
        src/main/console/console_log.hpp
        src/main/console/log.hpp
        src/test/audio_manager/audio_manager_tests.cpp
        src/main/task_manager/task_allocator.cpp
        src/main/task_manager/task_allocator.hpp)

target_link_libraries(audio_manager_test
        ST_loaders
        ST_util
        ST_message_bus
        gtest
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
        ${SDL2_TTF_LIBRARY}
        ${SDL2_MIXER_LIBRARY})

include_directories(audio_manager_test ../ST_engine/src/test)

add_executable(console_test
        src/main/defs.hpp
        src/main/console/log.hpp
        src/main/console/console.cpp
        src/main/console/console.hpp
        src/main/console/console_log.cpp
        src/main/console/console_log.hpp
        src/test/console/console_tests.cpp)

include_directories(console_test ../ST_engine/src/test)

target_link_libraries(console_test
        ST_loaders
        ST_util
        ST_message_bus
        gtest
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
        ${SDL2_TTF_LIBRARY}
        ${SDL2_MIXER_LIBRARY})

add_executable(display_manager_test
        src/main/defs.hpp
        src/main/display_manager/display_manager.cpp
        src/main/display_manager/display_manager.hpp
        src/test/display_manager/display_manager_tests.cpp
        src/main/console/console_log.cpp
        src/main/console/console_log.hpp
        src/main/console/log.hpp
        src/main/task_manager/task_allocator.cpp
        src/main/task_manager/task_allocator.hpp)

include_directories(display_manager_test ../ST_engine/src/test)

target_link_libraries(display_manager_test
        ST_loaders
        ST_util
        ST_message_bus
        gtest
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
        ${SDL2_TTF_LIBRARY}
        ${SDL2_MIXER_LIBRARY})

add_executable(entity_test
        src/main/game_manager/level/entity.hpp
        src/test/game_manager/level/entity_tests.cpp)

target_link_libraries(entity_test
        ST_util
        -pthread
        gtest)

add_executable(level_test
        src/main/game_manager/level/entity.hpp
        src/main/game_manager/level/light.cpp
        src/main/game_manager/level/light.hpp
        src/main/game_manager/level/level.cpp
        src/main/game_manager/level/level.hpp
        src/test/game_manager/level/level_tests.cpp
        src/main/console/console.cpp
        src/main/console/console.hpp
        src/main/console/console_log.cpp
        src/main/console/console_log.hpp)

target_link_libraries(level_test
        ST_util
        gtest
        ST_message_bus
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
        ${SDL2_TTF_LIBRARY}
        ${SDL2_MIXER_LIBRARY})

add_executable(lua_backend_test
        src/main/game_manager/level/entity.hpp
        src/main/game_manager/level/light.cpp
        src/main/game_manager/level/light.hpp
        src/main/game_manager/level/level.cpp
        src/main/game_manager/level/level.hpp
        src/main/console/console.cpp
        src/main/console/console.hpp
        src/main/console/console_log.cpp
        src/main/console/console_log.hpp
        src/main/game_manager/level/text.cpp
        src/main/game_manager/level/text.hpp
        src/main/main/timer.cpp
        src/main/main/timer.hpp
        src/test/game_manager/lua_backend/lua_backend_tests.cpp
        src/test/game_manager/lua_backend/game_manager_mock.hpp)

target_link_libraries(lua_backend_test
        ST_util
        ST_message_bus
        gtest
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
        ${SDL2_TTF_LIBRARY}
        ${SDL2_MIXER_LIBRARY}
        lua)

include_directories(lua_backend_test ../ST_engine/src
        ../ST_engine/src/test ../ST_loaders/include)

add_executable(task_manager_test
        src/main/task_manager/task_manager.cpp
        src/main/task_manager/task_manager.hpp
        src/main/task_manager/task.hpp
        src/main/task_manager/task_allocator.cpp
        src/main/task_manager/task_allocator.hpp
        src/test/task_manager/task_manager_tests.cpp
        ST_message_bus/src/main/message.hpp
        src/main/console/console_log.cpp
        src/main/console/console_log.hpp)

target_link_libraries(task_manager_test
        ST_util
        ST_message_bus
        gtest
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
        ${SDL2_TTF_LIBRARY}
        ${SDL2_MIXER_LIBRARY})

set(ALL_TESTS
        console_test
        audio_manager_test
        display_manager_test
        asset_manager_test
        entity_test
        level_test
        lua_backend_test
        task_manager_test)

set(RUN_ON_BUILD_TESTS
        console_test
        asset_manager_test
        entity_test
        level_test
        lua_backend_test
        task_manager_test)

gtest_add_tests(TARGET ${ALL_TESTS})
add_dependencies(ST_engine ${ALL_TESTS})
set_target_properties(${ALL_TESTS} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/src/test/test_resources)
set_target_properties(ST_engine PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/gamedata)

#[[
#Run the tests on each build
foreach(loop_var ${RUN_ON_BUILD_TESTS})
    add_custom_command(
            TARGET ST_engine
            POST_BUILD
            COMMAND ${loop_var}
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src/test/test_resources
    )
endforeach()]]
