#!/bin/bash
g++ -o  main -Wall -I. -O3 main.cpp timer.cpp assets_manager.cpp level.cpp math.cpp level_manager.cpp audio_manager.cpp renderer.cpp input_manager.cpp font_cache.cpp lua_manager.cpp display_manager.cpp physics.cpp entity.cpp console.cpp -std=gnu++11  -lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf -ldl -llua5.3 -lluajit-5.1 -lGLEW 
