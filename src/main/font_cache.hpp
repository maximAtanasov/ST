#ifndef QFC_DEF
#define QFC_DEF

#include "defs.hpp"
#include <list>
#include <tuple>

#ifndef CACHEHASH_DEF
#define CACHEHASH_DEF

typedef std::tuple<std::string, std::string, int> font_cache_tuple;
namespace std{
    template <> struct hash<font_cache_tuple>{
          std::size_t operator()(const font_cache_tuple& k) const{
          using std::size_t;
          using std::hash;
          using std::string;
          return ((hash<string>()(std::get<0>(k)) ^ (hash<string>()(std::get<1>(k)) << 1)) >> 1) ^ (hash<int>()(std::get<2>(k)) << 1);
        }
    };
}

bool operator==(const std::tuple<std::string, std::string, int>& tpl1, const std::tuple<std::string, std::string, int>& tpl2);

typedef std::pair<font_cache_tuple, SDL_Texture*> key_pair;
typedef std::list<key_pair> cache_list;
typedef std::unordered_map<font_cache_tuple, cache_list::iterator> cache_hash;

#endif

class font_cache{
    private:
        int entries;
        SDL_Texture* last_check;
        cache_list cache;
        cache_hash hash;
        int cache_size;
        void move_to_front(std::list<key_pair>& list,std::list<key_pair>::iterator element);
    public:
        explicit font_cache(int);
        ~font_cache();
        void cache_string(std::string, SDL_Texture*, std::string, int);
        bool str_is_cached(std::string, std::string, int);
        SDL_Texture* return_cache();
};

#endif
