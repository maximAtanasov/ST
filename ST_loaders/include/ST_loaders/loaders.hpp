/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */


#ifndef LOADERS_LIBRARY_H
#define LOADERS_LIBRARY_H

#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <unordered_map>
#include <iostream>
#include <sstream>

namespace ST {

    struct assets_named{
        std::unordered_map<std::string, SDL_Surface *> surfaces;
        std::unordered_map<std::string, Mix_Chunk *> chunks;
        std::unordered_map<std::string, Mix_Music *> music;
    };

    ST::assets_named *unpack_binary(const std::string &path);
    void pack_to_binary(const std::string &path, std::vector<std::string> args);
    int unpack_binary_to_disk(const std::string &path);
    std::string get_file_extension(const std::string &filename);
    bool replace_string(std::string &str, const std::string &from, const std::string &to);
}
#endif