/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#ifndef QFCGL_DEF
#define QFCGL_DEF

#include <defs.hpp>
#include <GL/glew.h>
#include <renderers/renderer_opengl_immediate/texture_opengl_immediate.hpp>
#include <list>
#include <tuple>


#ifndef CACHEHASH_DEF
#define CACHEHASH_DEF
typedef std::tuple<std::string, std::string, int> font_cache_opengl_tuple;



namespace std {
    template <>
      struct hash<font_cache_opengl_tuple>{
        std::size_t operator()(const font_cache_opengl_tuple& k) const{
          using std::size_t;
          using std::hash;
          using std::string;

          return ((hash<string>()(std::get<0>(k)) ^ (hash<string>()(std::get<1>(k)) << 1)) >> 1) ^ (hash<int>()(std::get<2>(k)) << 1);
        }
      };
}

typedef std::pair<font_cache_opengl_tuple, texture_gl_legacy*> key_pair;
typedef std::list<key_pair> cache_list;
typedef std::unordered_map<font_cache_opengl_tuple, cache_list::const_iterator> cache_hash;

#endif
class font_cache_opengl{
    private:
        int entries;
        texture_gl_legacy* last_check;
        cache_list cache;
        cache_hash hash;
        int cache_size;
        void move_to_front(std::list<key_pair>& list,std::list<key_pair>::const_iterator element);
    public:
        explicit font_cache_opengl(int);
        ~font_cache_opengl();
        void cache_string(std::string, texture_gl_legacy*, std::string, int);
        bool str_is_cached(std::string, std::string, int);
        texture_gl_legacy* return_cache();
};



#endif
