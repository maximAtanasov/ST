/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#ifndef FONT_CACHE_DEF
#define FONT_CACHE_DEF

#include <list>
#include <tuple>
#include <ST_util/bytell_hash_map.hpp>
#include <SDL_system.h>

#ifndef CACHEHASH_DEF
#define CACHEHASH_DEF

typedef std::tuple<std::string, uint16_t> font_cache_tuple;
namespace std {

    ///defines a hash operator for the font_cache_tuple type - this is needed because we use it in a ska::bytell_hash_map
    template<>
    struct hash<font_cache_tuple> {
        std::size_t operator()(const font_cache_tuple &k) const {
            using std::uint16_t;
            using std::hash;
            using std::string;
            return ((hash<string>()(std::get<0>(k)) ^ (hash<uint16_t>()(std::get<1>(k)) << 1)) >> 1);
        }
    };
}

//redefine the equals operator for two font caches tuples <string, string, int>
bool operator==(const std::tuple<std::string, std::string, int> &tpl1,
                const std::tuple<std::string, std::string, int> &tpl2);

//A few typedefs to make working with these types easier.
typedef std::pair<font_cache_tuple, SDL_Texture *> key_pair;
typedef std::list<key_pair> cache_list;
typedef ska::bytell_hash_map<font_cache_tuple, cache_list::iterator> cache_hash;

#endif

///A LRU Cache that caches rendered strings, it is used in the drawTextNormal() method of the renderer class
/**
 * Caches textures and the string, font+size used to render them.
 */
namespace ST::renderer_sdl {
    namespace font_cache {

        void move_to_front(std::list<key_pair> &list, std::list<key_pair>::iterator element);

        void set_max(uint32_t max);

        void cache_string(const std::string &str, SDL_Texture *texture, uint16_t font);

        SDL_Texture *get_cached_string(const std::string &str, uint16_t font);

        void clear();

        void close();
    };
}

#endif // FONT_CACHE_DEF
