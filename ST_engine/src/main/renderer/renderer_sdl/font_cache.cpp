/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <renderer/renderer_sdl/font_cache.hpp>

static uint32_t entries = 0;
static cache_list cache;
static cache_hash hash;
static uint32_t cache_size = 0;

/**
 * Moves a an element to the front of the list.
 * @param list The list.
 * @param element The element to move.
 */
void ST::renderer_sdl::font_cache::move_to_front(std::list<key_pair>& list,std::list<key_pair>::iterator element){
    if(element != list.begin()) {
        list.splice(list.begin(), list, element, std::next(element));
    }
}

/**
 * Sets the maximum cache size.
 * @param max The maximum number of Cached rendered textures that can be stored.
 */
void ST::renderer_sdl::font_cache::set_max(uint32_t max) {
    cache_size = max;
}

/**
 * @param STR The string to search for.
 * @param FONT The font of the text.
 * @param SIZE The size of the font that the text was rendered at.
 * @return The cached string corresponding to the give parameters or nullptr if it was not found
 */
SDL_Texture* ST::renderer_sdl::font_cache::get_cached_string(std::string str, std::string font, uint8_t size){
    font_cache_tuple temp = std::make_tuple(str, font, size);
    if(hash.find(temp) != hash.end()){
        move_to_front(cache, hash[temp]);
        return hash.at(temp)->second;;
    }
    return nullptr;
}

/**
 * Caches a string of text rendered with a given font at a given size.
 * @param STR The string to cache.
 * @param TEXTURE The texture to cache.
 * @param FONT The font to cache.
 * @param SIZE The size to cache.
 */
void ST::renderer_sdl::font_cache::cache_string(std::string str, SDL_Texture* texture, std::string font, uint8_t size){
    font_cache_tuple temp = std::make_tuple(str, font, size);
    cache.push_front(std::make_pair(temp, texture));
    hash[temp] = cache.begin();
    entries++;
    if(entries > cache_size){
        hash.erase(cache.back().first);
        SDL_DestroyTexture(cache.back().second);
        cache.pop_back();
        entries--;
    }
}

/**
 * Destroys the font_cache object and frees all cached textures.
 */
void ST::renderer_sdl::font_cache::close(){
    for(auto& i : cache){
        SDL_DestroyTexture(i.second);
    }
}

void ST::renderer_sdl::font_cache::clear() {
    cache.clear();
    hash.clear();
}
