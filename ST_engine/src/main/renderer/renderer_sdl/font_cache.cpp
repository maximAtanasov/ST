/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <renderer/renderer_sdl/font_cache.hpp>

/**
 * Moves a an element to the front of the list.
 * @param list The list.
 * @param element The element to move.
 */
void font_cache::move_to_front(std::list<key_pair>& list,std::list<key_pair>::iterator element){
    if(element != list.begin()) {
        list.splice(list.begin(), list, element, std::next(element));
    }
}

/**
 * Sets the maximum cache size.
 * @param max The maximum number of Cached rendered textures that can be stored.
 */
void font_cache::set_max(int max) {
    cache_size = max;
}

/**
 * @return The last searched for texture.
 */
SDL_Texture* font_cache::return_cache() {
    return last_check;
}

/**
 * Tells if a string with the given font and at the given size is in the cache.
 *
 * @param STR The string to search for.
 * @param FONT The font of the text.
 * @param SIZE The size of the font that the text was rendered at.
 * @return True if in cache, false otherwise.
 */
bool font_cache::str_is_cached(std::string STR, std::string FONT, int SIZE){
    font_cache_tuple temp = std::make_tuple(STR, FONT, SIZE);
    if(hash.find(temp) != hash.end()){
        last_check = hash[temp]->second;
        move_to_front(cache, hash[temp]);
        return true;
    }
    return false;
}

/**
 * Caches a string of text rendered with a given font at a given size.
 * @param STR The string to cache.
 * @param TEXTURE The texture to cache.
 * @param FONT The font to cache.
 * @param SIZE The size to cache.
 */
void font_cache::cache_string(std::string STR, SDL_Texture* TEXTURE, std::string FONT, int SIZE){
    font_cache_tuple temp = std::make_tuple(STR, FONT, SIZE);
    cache.push_front(std::make_pair(temp, TEXTURE));
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
font_cache::~font_cache(){
    for(auto& i : cache){
        SDL_DestroyTexture(i.second);
    }
}
