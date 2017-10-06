#include "assets_manager.hpp"
#include "renderer.hpp"

assets_manager::assets_manager(){
    //empty constructor
}

int assets_manager::initialize(){
    load_assets_from_list("levels/assets_global.list");
    return 0;
}

int assets_manager::load_asset(std::string path){
    if(count[path] > 0){
        count[path]++;
        return 0;
    }
    char extention[4];
    int length = path.length();
    extention[0] = path.c_str()[length - 3];
    extention[1] = path.c_str()[length - 2];
    extention[2] = path.c_str()[length - 1];
    extention[3] = 0;
    gConsole.write("Loading " + path);
    std::hash<std::string> hash_f;
    std::cout << "Loading " << path << "\n";

    //Handle the different extentions - currently png, wav, mp3, ttf
    if(strcmp(extention, "png") == 0){
        SDL_Surface* temp1 = IMG_Load(path.c_str());
        if(temp1 != NULL){
            int string_hash = hash_f(path);
            all_assets.textures[string_hash] = SDL_CreateTextureFromSurface(gRenderer.get_renderer(), temp1);
            SDL_FreeSurface(temp1);
            count[path]++;
        }else{
            gConsole.write("File " + path + " not found");
            std::cerr << "File " << path << " not found!\n";
        }
    }else if(strcmp(extention, "wav") == 0){
        Mix_Chunk* temp1 = Mix_LoadWAV(path.c_str());
        if (temp1 != NULL){
            all_assets.sounds.chunks[path] = temp1;
            count[path]++;
        }
        else{
            gConsole.write("File " + path + " not found");
            std::cerr << "File " << path << " not found!\n";
        }
    }else if(strcmp(extention, "mp3") == 0){
        Mix_Music* temp1 = Mix_LoadMUS(path.c_str());
        if (temp1 != NULL) {
            all_assets.sounds.music[path] = Mix_LoadMUS(path.c_str());
        }
        else {
            gConsole.write("File " + path + " not found");
            std::cerr << "File " << path << " not found!\n";
        }
    }else{ //if file is a font
        std::vector<std::string> result;
        std::istringstream iss(path);
        for(std::string path; iss >> path;)
            result.push_back(path);
        int size;
        try{
            std::stringstream convert(result.at(1));
            convert >> size;
        }catch(const std::out_of_range& e){
            gConsole.write("Font size not specified");
            fprintf(stderr, "Font size not specified\n");
            return -1;
        }
        std::string font = result.at(0);
        TTF_Font* tempFont = TTF_OpenFont(font.c_str(), size);
        if(tempFont != NULL){
            std::string font_and_size = font + result.at(1);
            all_assets.fonts_assets.fonts[font_and_size] = tempFont;
            cache_font(tempFont, font_and_size);
            count[path]++;
        }else{
            gConsole.write("File " + font + " not found");
            std::cerr << "File " << font << " not found!" << "\n";
        }
    }
    return 0;
}

//do not confuse this method with the font_cache class, they have nothing in common, this caches fonts to be used
//with ASCII strings, font_cache is a LRU cache that works with any string
void assets_manager::cache_font(TTF_Font* Font, std::string font_and_size){
    SDL_Color color_font = {255, 255, 255, 255};
    char temp[2];
    temp[1] = 0;
        std::vector<SDL_Texture*> tempVector;
        for(int j = 32; j < 127; j++){
            temp[0] = j;
            SDL_Surface* glyph = TTF_RenderUTF8_Blended(Font, temp, color_font);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer.get_renderer(), glyph);
            tempVector.push_back(texture);
            SDL_FreeSurface(glyph);
        }
        all_assets.fonts_assets.cache[font_and_size] = tempVector;
}

int assets_manager::load_assets_from_list(std::string path){
    std::ifstream file;
    file.open(path.c_str());
    if(file.is_open()){
        std::string temp;
        while(!file.eof()){
            getline(file, temp);
            if(!temp.empty())
                load_asset(temp);
        }
        file.close();
    }
    else{
        std::cerr << "File " +  path + " not found!\n";
        return -1;
    }
    return 0;
}

int assets_manager::unload_assets_from_list(std::string path){
    std::ifstream file;
    file.open(path.c_str());
    if(file.is_open()){
        std::string temp;
        while(!file.eof()){
            getline(file, temp);
            if(!temp.empty())
                unload_asset(temp);
        }
        file.close();
    }
    else{
        std::cerr << "File " +  path + " not found!\n";
        return -1;
    }
    return 0;
}

int assets_manager::unload_asset(std::string path){
    if(count[path] > 1){
        count[path]--;
        return 0;
    }
    char extention[4];
    int length = path.length();
    extention[0] = path.c_str()[length - 3];
    extention[1] = path.c_str()[length - 2];
    extention[2] = path.c_str()[length - 1];
    extention[3] = 0;
    gConsole.write("Loading " + path);
    std::cout << "Loading " << path << "\n";
    if(strcmp(extention, "png") == 0){
        std::hash<std::string> hash_f;
        int string_hash = hash_f(path);
        SDL_DestroyTexture(all_assets.textures[string_hash]);
        count[path]--;
    }else if(strcmp(extention, "wav") == 0){
        Mix_FreeChunk(all_assets.sounds.chunks[path]);
        count[path]--;
    }else if(strcmp(extention, "mp3") == 0){
        Mix_FreeMusic(all_assets.sounds.music[path]);
        count[path]--;
    }else{ //if file is a font
        TTF_CloseFont(all_assets.fonts_assets.fonts[path]);
        for(auto k = all_assets.fonts_assets.cache[path].begin(); k != all_assets.fonts_assets.cache[path].end(); k++)
            SDL_DestroyTexture(*k);
        count[path]--;
    }
    return 0;
}
