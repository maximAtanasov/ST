/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */


#ifndef LOADERS_LIBRARY_H
#define LOADERS_LIBRARY_H

#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <iostream>
#include <sstream>
#include <ST_util/string_util.hpp>
#include <ST_util/bytell_hash_map.hpp>

namespace ST {

    ///This struct contains assets just like the regular ST::assets, except it uses asset names as keys instead of hashes.
    struct assets_named{
        ska::bytell_hash_map<std::string, SDL_Surface *> surfaces;
        ska::bytell_hash_map<std::string, Mix_Chunk *> chunks;
        ska::bytell_hash_map<std::string, Mix_Music *> music;
    };

    ST::assets_named* unpack_binary(const std::string &path);
    void pack_to_binary(const std::string &path, std::vector<std::string> args);
    int8_t unpack_binary_to_disk(const std::string &path);
    std::string get_file_extension(const std::string &filename);
    int8_t add_to_binary(const std::string &binary_name, std::vector<std::string> args_);
}
#endif