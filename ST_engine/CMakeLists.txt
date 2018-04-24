# Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
# You may not use, distribute or modify this code.
# This code is proprietary and belongs to the "slavicTales"
# project. See LICENCE.txt in the root directory of the project.
#
# E-mail: atanasovmaksim1@gmail.com

cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(ST_engine)

set(CMAKE_CXX_STANDARD 11)

enable_testing()

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../cmake)

find_package(GTest REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(Lua REQUIRED)

include_directories(../ST_loaders/include)
include_directories(../ST_util/include)
include_directories(../ST_engine/src/main)
include_directories(${LUA_INCLUDE_DIR})
include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_IMAGE_INCLUDE_DIR})
include_directories(${SDL2_MIXER_INCLUDE_DIR})
include_directories(${SDL2_TTF_INCLUDE_DIR})

add_executable(ST_engine
        src/main/assets_manager/assets.hpp
        src/main/assets_manager/assets_manager.cpp
        src/main/assets_manager/assets_manager.hpp
        src/main/util/atomic_queue/atomicops.h
        src/main/audio_manager/audio_manager.cpp
        src/main/audio_manager/audio_manager.hpp
        src/main/game_manager/level/camera.hpp
        src/main/util/atomic_queue/concurrentqueue.h
        src/main/console/console.cpp
        src/main/console/console.hpp
        src/main/defs.hpp
        src/main/display_manager/display_manager.cpp
        src/main/display_manager/display_manager.hpp
        src/main/drawing_manager/drawing_manager.cpp
        src/main/drawing_manager/drawing_manager.hpp
        src/main/game_manager/level/entity.cpp
        src/main/game_manager/level/entity.hpp
        src/main/renderer/renderer_sdl/font_cache.cpp
        src/main/renderer/renderer_sdl/font_cache.hpp
        src/main/game_manager/game_manager.cpp
        src/main/game_manager/game_manager.hpp
        src/main/input_manager/input_manager.cpp
        src/main/input_manager/input_manager.hpp
        src/main/game_manager/level/level.cpp
        src/main/game_manager/level/level.hpp
        src/main/game_manager/level/level_data.hpp
        src/main/game_manager/level/light.cpp
        src/main/game_manager/level/light.hpp
        src/main/main/main.cpp
        src/main/main/main.hpp
        src/main/message_bus/message_bus.cpp
        src/main/message_bus/message_bus.hpp
        src/main/physics_manager/physics_manager.cpp
        src/main/physics_manager/physics_manager.hpp
        src/main/util/atomic_queue/readerwriterqueue.h
        src/main/renderer/renderer_sdl/renderer_sdl.cpp
        src/main/renderer/renderer_sdl/renderer_sdl.hpp
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
        src/main/message_bus/subscriber.cpp
        src/main/message_bus/subscriber.hpp
        src/main/message_bus/message.hpp
        src/main/message_bus/message_allocator.cpp
        src/main/message_bus/message_allocator.hpp
        src/main/message_bus/message_types.hpp)

target_link_libraries(ST_engine
        ST_util
        ST_loaders
        ${LUA_LIBRARY}
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
        src/main/message_bus/message_bus.cpp
        src/main/message_bus/message_bus.hpp
        src/main/task_manager/task.hpp
        src/main/task_manager/task_manager.cpp
        src/main/task_manager/task_manager.hpp
        src/main/console/console_log.cpp
        src/main/console/console_log.hpp
        src/main/console/log.hpp
        src/test/assets_manager/assets_manager_tests.cpp
        src/main/message_bus/subscriber.cpp
        src/main/message_bus/subscriber.hpp
        src/main/message_bus/message.hpp
        src/main/message_bus/message_allocator.cpp
        src/main/message_bus/message_allocator.hpp
        src/main/message_bus/message_types.hpp)

include_directories(asset_manager_test ../ST_engine/src/test)


target_link_libraries(asset_manager_test
        ST_loaders
        ST_util
        ${GTEST_LIBRARIES}
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
        ${SDL2_TTF_LIBRARY}
        ${SDL2_MIXER_LIBRARY}
        ${GLEW_LIBRARIES}
        ${OPENGL_LIBRARIES}
        ${LUA_LIBRARY})


add_executable(audio_manager_test
        src/main/assets_manager/assets.hpp
        src/main/audio_manager/audio_manager.cpp
        src/main/audio_manager/audio_manager.hpp
        src/main/defs.hpp
        src/main/message_bus/message_bus.cpp
        src/main/message_bus/message_bus.hpp
        src/main/task_manager/task.hpp
        src/main/task_manager/task_manager.cpp
        src/main/task_manager/task_manager.hpp
        src/main/console/console_log.cpp
        src/main/console/console_log.hpp
        src/main/console/log.hpp
        src/test/audio_manager/audio_manager_tests.cpp
        src/main/message_bus/subscriber.cpp
        src/main/message_bus/subscriber.hpp
        src/main/message_bus/message.hpp
        src/main/message_bus/message_allocator.cpp
        src/main/message_bus/message_allocator.hpp
        src/main/message_bus/message_types.hpp)

target_link_libraries(audio_manager_test
        ST_loaders
        ST_util
        ${GTEST_LIBRARIES}
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
        ${SDL2_TTF_LIBRARY}
        ${SDL2_MIXER_LIBRARY}
        ${LUA_LIBRARY})

include_directories(audio_manager_test ../ST_engine/src/test)

add_executable(console_test
        src/main/defs.hpp
        src/main/message_bus/message_bus.cpp
        src/main/message_bus/message_bus.hpp
        src/main/console/console_log.cpp
        src/main/console/console_log.hpp
        src/main/console/log.hpp
        src/main/message_bus/subscriber.cpp
        src/main/message_bus/subscriber.hpp
        src/main/message_bus/message.hpp
        src/main/message_bus/message_allocator.cpp
        src/main/message_bus/message_allocator.hpp
        src/main/message_bus/message_types.hpp
        src/main/console/console.cpp
        src/main/console/console.hpp
        src/test/console/console_tests.cpp)

include_directories(console_test ../ST_engine/src/test)

target_link_libraries(console_test
        ST_loaders
        ST_util
        ${GTEST_LIBRARIES}
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
        ${SDL2_TTF_LIBRARY}
        ${SDL2_MIXER_LIBRARY})

add_executable(display_manager_test
        src/main/defs.hpp
        src/main/display_manager/display_manager.cpp
        src/main/display_manager/display_manager.hpp
        src/test/display_manager/display_manager_tests.cpp
        src/main/message_bus/message_bus.cpp
        src/main/message_bus/message_bus.hpp
        src/main/console/console_log.cpp
        src/main/console/console_log.hpp
        src/main/console/log.hpp
        src/main/message_bus/subscriber.cpp
        src/main/message_bus/subscriber.hpp
        src/main/message_bus/message.hpp
        src/main/message_bus/message_allocator.cpp
        src/main/message_bus/message_allocator.hpp
        src/main/message_bus/message_types.hpp)

include_directories(display_manager_test ../ST_engine/src/test)

target_link_libraries(display_manager_test
        ST_loaders
        ST_util
        ${GTEST_LIBRARIES}
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
        ${SDL2_TTF_LIBRARY}
        ${SDL2_MIXER_LIBRARY})

#Run all our tests on every build
set(ALL_TESTS asset_manager_test audio_manager_test console_test display_manager_test)
gtest_add_tests(TARGET ${ALL_TESTS})
add_dependencies(ST_engine ${ALL_TESTS})
set_target_properties(asset_manager_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/src/test/assets_manager)
set_target_properties(ST_engine PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/gamedata)

#This loop will run all of our tests
foreach(loop_var ${ALL_TESTS} total)
    #The test for the asset_manager needs to be run in a specific directory
    if(${loop_var} STREQUAL "asset_manager_test")
    add_custom_command(
            TARGET ST_engine
            POST_BUILD
            COMMAND ${loop_var}
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src/test/assets_manager
    )
    elseif(NOT ${loop_var} STREQUAL "total")
    add_custom_command(
            TARGET ST_engine
            POST_BUILD
            COMMAND ${loop_var}
    )
    endif()
endforeach()