# This file is part of the "ST" project.
# You may use, distribute or modify this code under the terms
# of the GNU General Public License version 2.
# See LICENCE.txt in the root directory of the project.
#
# Author: Maxim Atanasov
# E-mail: maxim.atanasov@protonmail.com

cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(ST_engine)

set(CMAKE_CXX_STANDARD 14)

include(GoogleTest)
enable_testing()

add_subdirectory(ST_renderer)
add_subdirectory(ST_message_bus)
add_subdirectory(ST_dev_console)
add_subdirectory(ST_task_manager)
add_subdirectory(ST_audio_manager)
add_subdirectory(ST_assets_manager)
add_subdirectory(ST_input_manager)
add_subdirectory(ST_window_manager)

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
include_directories(../ST_engine/ST_dev_console/include)
include_directories(../ST_engine/ST_task_manager/include)
include_directories(../ST_engine/ST_window_manager/include)
include_directories(../ST_engine/ST_audio_manager/include)
include_directories(../ST_engine/ST_assets_manager/include)
include_directories(../ST_engine/ST_input_manager/include)
include_directories(../external/googletest/googletest/include)
include_directories(../external/googletest/googlemock/include)
include_directories(../external/lua/lua-5.3.0/src)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_IMAGE_INCLUDE_DIR})
include_directories(${SDL2_MIXER_INCLUDE_DIR})
include_directories(${SDL2_TTF_INCLUDE_DIR})

add_executable(ST_engine
        src/main/game_manager/level/camera.hpp
        src/main/drawing_manager/drawing_manager.cpp
        src/main/drawing_manager/drawing_manager.hpp
        src/main/game_manager/level/entity.hpp
        src/main/game_manager/game_manager.cpp
        src/main/game_manager/game_manager.hpp
        src/main/game_manager/level/level.cpp
        src/main/game_manager/level/level.hpp
        src/main/game_manager/level/light.cpp
        src/main/game_manager/level/light.hpp
        src/main/main/main.cpp
        src/main/main/main.hpp
        src/main/physics_manager/physics_manager.cpp
        src/main/physics_manager/physics_manager.hpp
        src/main/game_manager/level/text.hpp
        src/main/main/timer.cpp
        src/main/main/timer.hpp
        src/main/main/fps.cpp
        src/main/main/fps.hpp
        src/main/game_manager/lua_backend/lua_backend.cpp
        src/main/game_manager/lua_backend/lua_backend.hpp
        src/main/game_manager/level/text.cpp)

target_link_libraries(ST_engine
        ST_util
        ST_renderer
        ST_message_bus
        ST_dev_console
        ST_task_manager
        ST_assets_manager
        ST_loaders
        ST_audio_manager
        ST_input_manager
        ST_window_manager
        lua
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
        ${SDL2_TTF_LIBRARY}
        ${SDL2_MIXER_LIBRARY}
       )

add_executable(entity_test
        src/main/game_manager/level/entity.hpp
        src/test/game_manager/level/entity_tests.cpp)

target_link_libraries(entity_test
        ST_util
        gtest_main)

add_executable(level_test
        src/main/game_manager/level/entity.hpp
        src/main/game_manager/level/light.cpp
        src/main/game_manager/level/light.hpp
        src/main/game_manager/level/level.cpp
        src/main/game_manager/level/level.hpp
        src/test/game_manager/level/level_tests.cpp)

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

add_executable(ST_engine_integration_test
        src/test/integration/ST_engine_integration_tests.cpp
        src/main/game_manager/level/camera.hpp
        src/main/drawing_manager/drawing_manager.cpp
        src/main/drawing_manager/drawing_manager.hpp
        src/main/game_manager/level/entity.hpp
        src/main/game_manager/game_manager.cpp
        src/main/game_manager/game_manager.hpp
        src/main/game_manager/level/level.cpp
        src/main/game_manager/level/level.hpp
        src/main/game_manager/level/light.cpp
        src/main/game_manager/level/light.hpp
        src/main/main/main.hpp
        src/main/physics_manager/physics_manager.cpp
        src/main/physics_manager/physics_manager.hpp
        src/main/game_manager/level/text.hpp
        src/main/main/timer.cpp
        src/main/main/timer.hpp
        src/main/main/fps.cpp
        src/main/main/fps.hpp
        src/main/game_manager/lua_backend/lua_backend.cpp
        src/main/game_manager/lua_backend/lua_backend.hpp
        src/main/game_manager/level/text.cpp)

target_link_libraries(ST_engine_integration_test
        ST_util
        ST_renderer
        ST_message_bus
        ST_dev_console
        ST_task_manager
        ST_audio_manager
        ST_assets_manager
        ST_loaders
        ST_input_manager
        ST_window_manager
        lua
        gtest
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
        ${SDL2_TTF_LIBRARY}
        ${SDL2_MIXER_LIBRARY})


set(ALL_TESTS
        entity_test
        level_test
        lua_backend_test
        ST_engine_integration_test)

set(RUN_ON_BUILD_TESTS
        entity_test
        level_test
        lua_backend_test)

gtest_add_tests(TARGET ${ALL_TESTS})
add_dependencies(ST_engine ${ALL_TESTS})
set_target_properties(${ALL_TESTS} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/src/test/test_resources)
set_target_properties(ST_engine PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/gamedata)
set_target_properties(ST_engine_integration_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/gamedata)


#Run the tests on each build
foreach(loop_var ${RUN_ON_BUILD_TESTS})
    add_custom_command(
            TARGET ST_engine
            POST_BUILD
            COMMAND ${loop_var}
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src/test/test_resources
    )
endforeach()

if(DEFINED ENV{CIRCLECI} OR DEFINED ENV{APPVEYOR})
    add_custom_command(
            TARGET ST_engine
            POST_BUILD
            COMMAND ST_engine_integration_test
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/gamedata
    )
endif()

