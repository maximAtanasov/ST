/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <renderers/renderer_sdl/font_cache.hpp>

//A LRU Cache that caches rendered strings, it is used in the drawTextNormal() method of the renderer class

font_cache::font_cache(int max){
    last_check = nullptr;
    cache_size = max;
    entries = 0;
}

void font_cache::move_to_front(std::list<key_pair>& list,std::list<key_pair>::iterator element){
    if(element != list.begin()) {
        list.splice(list.begin(), list, element, std::next(element));
    }
}

SDL_Texture* font_cache::return_cache() {
    return last_check;
}

bool font_cache::str_is_cached(std::string STR, std::string FONT, int SIZE){
    font_cache_tuple temp = std::make_tuple(STR, FONT, SIZE);
    if(hash.find(temp) != hash.end()){
        last_check = hash[temp]->second;
        move_to_front(cache, hash[temp]);
        return true;
    }
    return false;
}

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

font_cache::~font_cache(){
    for(auto& i : cache){
        SDL_DestroyTexture(i.second);
    }
}
