CC=clang++
SOURCES_GAME= main.o math.o audio_manager.o  assets_manager.o renderer.o level_manager.o level.o display_manager.o input_manager.o lua_manager.o entity.o physics.o console.o font_cache.o
SOURCES_MAIN=main.cpp
SOURCES_RENDERER=renderer.cpp
SOURCES_AUDIOMANAGER=audio_manager.cpp
SOURCES_LUAMANAGER=lua_manager.cpp
SOURCES_INPUTMANAGER=input_manager.cpp
SOURCES_ENTITIES=entity.cpp
SOURCES_PHYSICS=physics.cpp
SOURCES_LEVELMANAGER=level_manager.cpp
SOURCES_CONSOLE=console.cpp
SOURCES_ASSETSMANAGER=assets_manager.cpp
LIBS=-lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf -ldl -llua5.3 -lluajit-5.1 -lGLEW
CFLAGS= -std=gnu++11 -Wall -O0

renderer.o: $(SOURCES_RENDERER) renderer.hpp
	$(CC) -c $(SOURCES_RENDERER)  $(CFLAGS)

audio_manager.o: $(SOURCES_AUDIOMANAGER) audio_manager.hpp
	$(CC) -c $(SOURCES_AUDIOMANAGER)  $(CFLAGS)

assets_manager.o: $(SOURCES_ASSETSMANAGER) assets_manager.hpp
	$(CC) -c $(SOURCES_ASSETSMANAGER) $(CFLAGS)

lua_manager.o: $(SOURCES_LUAMANAGER) lua_manager.hpp
	$(CC) -c $(SOURCES_LUAMANAGER)  $(CFLAGS)

input_manager.o: $(SOURCES_INPUTMANAGER) input_manager.hpp
	$(CC) -c $(SOURCES_INPUTMANAGER)  $(CFLAGS)

entity.o: $(SOURCES_ENTITIES) entity.hpp
	$(CC) -c $(SOURCES_ENTITIES)  $(CFLAGS)

physics.o: $(SOURCES_PHYSICS) physics.hpp
	$(CC) -c $(SOURCES_PHYSICS)  $(CFLAGS)

level_manager.o: $(SOURCES_LEVELMANAGER) level_manager.hpp
	$(CC) -c $(SOURCES_LEVELMANAGER)  $(CFLAGS)

main.o: $(SOURCES_MAIN) main.hpp
	$(CC) -c $(SOURCES_MAIN) $(CFLAGS)

font_cache.o: font_cache.cpp font_cache.hpp
	$(CC) -c font_cache.cpp $(CFLAGS)

console.o: $(SOURCES_CONSOLE) console.hpp
	$(CC) -c $(SOURCES_CONSOLE)  $(CFLAGS)

display_manager.o: display_manager.cpp display_manager.hpp
	$(CC) -c display_manager.cpp  $(CFLAGS)

level.o: level.cpp level.hpp
	$(CC) -c level.cpp  $(CFLAGS)

math.o: math.cpp math.hpp
	$(CC) -c math.cpp $(CFLAGS)

game: $(SOURCES_GAME)
	$(CC) -o  main $(SOURCES_GAME) $(LIBS)  $(CFLAGS)

.PHONY: clean

clean:
	$(RM) main $(SOURCES_GAME)
