/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#ifndef ST_TEST_UTIL_HPP
#define ST_TEST_UTIL_HPP

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string>

void close_SDL();
void initialize_SDL();
bool compare_surfaces(SDL_Surface* expected, SDL_Surface* result);
long get_file_size(const std::string& path);
void copy_file(const std::string& src, const std::string& dest);

#endif //ST_TEST_UTIL_HPP
