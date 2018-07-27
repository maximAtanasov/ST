/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#ifndef SLAVICTALES_TEST_UTIL_HPP
#define SLAVICTALES_TEST_UTIL_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <string>

void close_SDL();
void initialize_SDL();
bool compare_surfaces(SDL_Surface* expected, SDL_Surface* result);
long get_file_size(const std::string& path);

#endif //SLAVICTALES_TEST_UTIL_HPP
