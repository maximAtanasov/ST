#ifndef ASSTS_DEF
#define ASSTS_DEF

#include <string>
#include <unordered_map>
#include <ST_util/map.hpp>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"


namespace ST {

    //A container that holds all asset types used in the game
    struct assets {
        std::unordered_map<size_t, SDL_Surface *> surfaces;
        std::unordered_map<std::string, TTF_Font *> fonts;
        std::unordered_map<size_t, Mix_Music *> music;
        ST::map<Mix_Chunk*>* chunks;
    };
}

#endif
