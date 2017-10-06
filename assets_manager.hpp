#ifndef ASST_DEF
#define ASST_DEF

#include "defs.hpp"

typedef struct font_assets{
    std::unordered_map<std::string, TTF_Font*> fonts;
    std::unordered_map<std::string, std::vector<SDL_Texture*>> cache;
} font_assets;

typedef struct sound_assets{
    std::unordered_map<std::string, Mix_Chunk*> chunks;
    std::unordered_map<std::string, Mix_Music*> music;
} sound_assets;

typedef struct assets{
    std::unordered_map<int, SDL_Texture*> textures;
    sound_assets sounds;
    font_assets fonts_assets;
} assets;

class assets_manager{
    private:
        assets all_assets;
        std::unordered_map<std::string, int> count;
    public:
        assets_manager();
        int initialize();
        int load_asset(std::string);
        int load_assets_from_list(std::string);
        int unload_asset(std::string);
        int unload_assets_from_list(std::string);
        void cache_font(TTF_Font*, std::string);
        inline assets* get_assets(){
            return &all_assets;
        }
        void close();
};

extern assets_manager gAssets_manager;

#endif
