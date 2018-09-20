/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#ifndef GLOBALDEFS_DEF
#define GLOBALDEFS_DEF

//I don't think this will ever fail
static_assert(sizeof(float) == 4, "floats are not 32 bits on this system!");

#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <ST_util/math.hpp>
#include <ST_util/atomic_queue/concurrentqueue.h>
#include <ST_util/atomic_queue/atomicops.h>
#include <ST_util/debug_switch.hpp>
#include <ST_util/bytell_hash_map.hpp>

#endif //GLOBALDEFS_DEF
