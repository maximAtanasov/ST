/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#ifndef ASSTS_DEF
#define ASSTS_DEF

#include <string>
#include <unordered_map>
#include <ST_util/map.hpp>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"


namespace ST {

    ///A container that holds all asset types used in the game
    struct assets {
        std::unordered_map<size_t, SDL_Surface *> surfaces;
        std::unordered_map<std::string, TTF_Font *> fonts;
        std::unordered_map<size_t, Mix_Music *> music;
        std::unordered_map<size_t, Mix_Chunk *> chunks;
    };
}

#endif
