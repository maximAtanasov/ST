#ifndef SLAVICTALES_TEST_UTIL_HPP
#define SLAVICTALES_TEST_UTIL_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

void close_SDL();
void initialize_SDL();
bool compare_surfaces(SDL_Surface* expected, SDL_Surface* result);

#endif //SLAVICTALES_TEST_UTIL_HPP
