#include "font_cache.hpp"
#include "main.hpp"

//A LRU Cache that caches rendered strings, it is used in the drawTextNormal() method of the renderer class

font_cache::font_cache(int max){
    lastCheck = NULL;
    CacheSize = max;
    entries = 0;
}

void font_cache::moveToFront(std::list<keyPair>& list,std::list<keyPair>::const_iterator element){
    if ( element != list.begin())
        list.splice(list.begin(), list, element, std::next(element));
}

bool font_cache::StrIsCached(std::string STR, std::string FONT, int SIZE){
    Tuple temp = std::make_tuple(STR, FONT, SIZE);
    if(Hash.find(temp) != Hash.end()){
        lastCheck = Hash[temp]->second;
        moveToFront(Cache, Hash[temp]);
        return true;
    }
    return false;
}

void font_cache::cacheString(std::string STR, SDL_Texture* TEXTURE, std::string FONT, int SIZE){
    Tuple temp = std::make_tuple(STR, FONT, SIZE);
    Cache.push_front(std::make_pair(temp, TEXTURE));
    Hash[temp] = Cache.begin();
    entries++;
    if(entries > CacheSize){
        Hash.erase(Cache.back().first);
        SDL_DestroyTexture(Cache.back().second);
        Cache.pop_back();
        entries--;
    }
}

font_cache::~font_cache(){
    for(auto i = Cache.begin(); i != Cache.end(); i++){
        SDL_DestroyTexture(i->second);
    }
}
