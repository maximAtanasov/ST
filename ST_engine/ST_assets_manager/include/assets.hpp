/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#ifndef ASSTS_DEF
#define ASSTS_DEF

#include <string>

#include <ST_util/bytell_hash_map.hpp>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>


namespace ST {

    ///A container that holds all asset types used in the game
    struct assets {
        ska::bytell_hash_map<size_t, SDL_Surface *> surfaces;
        ska::bytell_hash_map<size_t, TTF_Font *> fonts;
        ska::bytell_hash_map<size_t, Mix_Music *> music;
        ska::bytell_hash_map<size_t, Mix_Chunk *> chunks;
    };
}

#endif
