#ifndef QFC_DEF
#define QFC_DEF

#include "defs.hpp"
#include <list>
#include <tuple>

typedef std::tuple<std::string, std::string, int> Tuple;

namespace std {
    template <>
      struct hash<Tuple>{
        std::size_t operator()(const Tuple& k) const{
          using std::size_t;
          using std::hash;
          using std::string;

          return ((hash<string>()(std::get<0>(k)) ^ (hash<string>()(std::get<1>(k)) << 1)) >> 1) ^ (hash<int>()(std::get<2>(k)) << 1);
        }
    };
}

typedef std::pair<Tuple, SDL_Texture*> keyPair;
typedef std::list<keyPair> CacheList;
typedef std::unordered_map<Tuple, CacheList::const_iterator> CacheHash;

class font_cache{
    private:
        int entries;
        SDL_Texture* lastCheck;
        CacheList Cache;
        CacheHash Hash;
        int CacheSize;
        void ReplaceLRU(std::string, SDL_Texture*, std::string, int);
        void moveToFront(std::list<keyPair>& list,std::list<keyPair>::const_iterator element);
    public:
        font_cache(int);
        ~font_cache();
        void cacheString(std::string, SDL_Texture*, std::string, int);
        bool StrIsCached(std::string, std::string, int);
        inline SDL_Texture* ReturnCache(){
            return lastCheck;
        }
};



#endif
