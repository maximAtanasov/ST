#include <renderers/renderer_sdl/renderer_sdl.hpp>

int renderer_sdl::initialize(SDL_Window* window, int width, int height){
    gFont_cache = new font_cache(100);
    //initialize renderer
	this->window = window;
	this->width = width;
	this->height = height;
    sdl_renderer = SDL_CreateRenderer( this->window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(sdl_renderer, this->width, this->height);
    SDL_SetRenderDrawBlendMode(sdl_renderer, SDL_BLENDMODE_BLEND);
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ); //Linear texture filtering
    set_draw_color(0, 0, 0, 255);

    //initialize lights surfaces and texture
    lights_texture = SDL_CreateTexture(sdl_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 1920, 1080);
    if(lights_texture == nullptr){
        printf("%s\n", SDL_GetError());
    }else{
        SDL_SetTextureBlendMode(lights_texture, SDL_BLENDMODE_BLEND);
    }
    pixels = (Uint32*)malloc(width*height*sizeof(Uint32));
    return 0;
}

int renderer_sdl::initialize_with_vsync(SDL_Window* window, int width, int height, bool vsync){
    gFont_cache = new font_cache(100);
    //initialize renderer
	if(vsync){
    	sdl_renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	}else{
		sdl_renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);
	}
    SDL_RenderSetLogicalSize(sdl_renderer, width, height);
 	SDL_SetRenderDrawBlendMode(sdl_renderer, SDL_BLENDMODE_BLEND);
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ); //Linear texture filtering
    set_draw_color(0, 0, 0, 255);
    lights_texture = SDL_CreateTexture(sdl_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 1920, 1080);
    if(lights_texture == nullptr) {
        printf("%s\n", SDL_GetError());
    }else{
        SDL_SetTextureBlendMode(lights_texture, SDL_BLENDMODE_BLEND);
    }
    return 0;
}

void renderer_sdl::draw_lights(Uint8 arg[1080][1920], const int w, const int h){
    int k = 0;
    for(Uint16 i = 0; i < w; i++){
        for(Uint16 j = 0; j < h; j++){
            pixels[k] = arg[i][j] << (Uint8)24;
            k++;
        };
    }
    SDL_UpdateTexture(lights_texture, nullptr, pixels, h*sizeof(Uint32));
    SDL_RenderCopy(sdl_renderer, lights_texture, nullptr, nullptr);
}

void renderer_sdl::close(){
    for (auto& it : fonts) {
        if (it.second != nullptr) {
            if (this->fonts[it.first] != nullptr) {
                for(auto& k : fonts_cache[it.first]) {
                    SDL_DestroyTexture(k);
                }
                this->fonts[it.first] = nullptr;
            }
            it.second = nullptr;
        }
    }
    for ( auto& it : textures){
        if(it.second != nullptr){
            SDL_DestroyTexture(textures[it.first]);
            it.second = nullptr;
        }
    }

    delete gFont_cache;
    SDL_DestroyTexture(lights_texture);
	gFont_cache = nullptr;
    SDL_DestroyRenderer(sdl_renderer);
    sdl_renderer = nullptr;
}

//Text fallback for non-english text -- works with cyrillic, expanded Latin (Spanish, German, etc..) and I guess all of UTF8
void renderer_sdl::draw_text_normal(std::string arg, std::string arg2, int x, int y, SDL_Color color_font , int size){
    std::string font_and_size = arg+std::to_string(size);
    TTF_Font* font = fonts[font_and_size];
    if(font != nullptr){
        int texW, texH;
        if(gFont_cache->str_is_cached(arg2, arg, size)){ //if the given string (with same size and font) is already cached, get it from cache
            SDL_Texture* texture = gFont_cache->return_cache();
            SDL_QueryTexture(texture, nullptr, nullptr, &texW, &texH);
            SDL_Rect Rect = {x, y, texW, texH};
            SDL_SetTextureColorMod(texture, color_font.r, color_font.g, color_font.b);
            SDL_RenderCopy(sdl_renderer, texture, nullptr, &Rect);
        }else{ //else create a texture, render it, and then cache it - this is costly, so pick a good cache size
            SDL_Surface* text = TTF_RenderUTF8_Blended(font, arg2.c_str(), color_font);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(sdl_renderer, text);
            SDL_QueryTexture(texture, nullptr, nullptr, &texW, &texH);
            SDL_Rect Rect = {x, y, texW, texH};
            SDL_SetTextureColorMod(texture, color_font.r, color_font.g, color_font.b);
            SDL_RenderCopy(sdl_renderer, texture, nullptr, &Rect);
            SDL_FreeSurface(text);
            gFont_cache->cache_string(arg2, texture, arg, size);
        }
    }
}

//This will draw text using cached glyphs - the fastest way possible, works only with english and relatively simple fonts
//(any complex cursive fonts won't be rendered properly)
void renderer_sdl::draw_text_cached(std::string arg, std::string arg2, int x, int y, SDL_Color color_font , int size){
    std::string font_and_size = arg+std::to_string(size);
    std::vector<SDL_Texture*> tempVector = fonts_cache[font_and_size];
    if(!tempVector.empty()){
        int texW, texH;
        int tempX = x;
        const char* arg3 = arg2.c_str();
        for(int j = 0; arg3[j] != 0; j++){
            SDL_Texture* texture = tempVector.at(static_cast<unsigned int>(arg3[j]-32));
            SDL_QueryTexture(texture, nullptr, nullptr, &texW, &texH);
            SDL_Rect Rect = {tempX, y, texW, texH};
            SDL_SetTextureColorMod(texture, color_font.r, color_font.g, color_font.b);
            SDL_RenderCopy(sdl_renderer, texture, nullptr, &Rect);
            tempX += texW;
        }
    }
}

//This is will decide which of the above text rendering methods to use based on the flag that is passed, if you are unsure if your
//font will render properly just pass in 0 (the default for all non-internal engine text)
void renderer_sdl::draw_text(std::string arg, std::string arg2, int x, int y, SDL_Color color_font , int size, int flag){
    if(flag == 1){
        draw_text_cached(arg, arg2, x, y, color_font, size);
    }else if(flag == 0){
        draw_text_normal(arg, arg2, x, y, color_font, size);
    }else{
        for(unsigned int i = 0; i < arg2.size(); i++) {
            if (arg2.at(i) > 126 || (arg2.at(i) < 32)) {
                draw_text_normal(arg, arg2, x, y, color_font, size);
                return;
            }
        }
        draw_text_cached(arg, arg2, x, y, color_font, size);
    }
}

void renderer_sdl::upload_surfaces(std::unordered_map<size_t, SDL_Surface*>* surfaces){
	if(surfaces != nullptr){
		this->surfaces_pointer = surfaces;
        for ( auto& it : *surfaces){
            if(it.second == nullptr && textures[it.first] != nullptr){
                SDL_DestroyTexture(textures[it.first]);
                textures[it.first] = nullptr;
            }
            else if(it.second != nullptr){
                if(this->textures[it.first] != nullptr){
                    SDL_DestroyTexture(this->textures[it.first]);
                    this->textures[it.first] = nullptr;
                }
                this->textures[it.first] = SDL_CreateTextureFromSurface(sdl_renderer, it.second);
            }
        }
    }
}

void renderer_sdl::upload_fonts(std::unordered_map<std::string, TTF_Font*>* fonts){
    if(fonts != nullptr){
		this->fonts_pointer = fonts;
        for ( auto& it : *fonts){
            if(it.second == nullptr){
                for(auto& k : fonts_cache[it.first]){
                    SDL_DestroyTexture(k);
                }
                this->fonts[it.first] = nullptr;
            }
            else if(it.second != nullptr){
                if(this->fonts[it.first] != nullptr){
                    for(auto& k : fonts_cache[it.first]){
                        SDL_DestroyTexture(k);
                    }
                    this->fonts[it.first] = nullptr;
                }
                this->fonts[it.first] = it.second;
                cache_font(this->fonts[it.first], it.first);
            }
        }
    }
}

//do not confuse this method with the font_cache class, they have nothing in common, this caches fonts to be used
//with ASCII strings, font_cache is a LRU cache that works with any string
void renderer_sdl::cache_font(TTF_Font* Font, std::string font_and_size){
    SDL_Color color_font = {255, 255, 255, 255};
    char temp[2];
    temp[1] = 0;
    std::vector<SDL_Texture*> tempVector;
    for(char j = 32; j < 127; j++){
        temp[0] = j;
        SDL_Surface* glyph = TTF_RenderUTF8_Blended(Font, temp, color_font);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(sdl_renderer, glyph);
        tempVector.push_back(texture);
        SDL_FreeSurface(glyph);
    }
    fonts_cache[font_and_size] = tempVector;
}


void renderer_sdl::vsync_on(){
	close();
	initialize_with_vsync(window, width, height, true);
	upload_surfaces(surfaces_pointer);
	upload_fonts(fonts_pointer);
}

void renderer_sdl::vsync_off(){
	close();
	initialize_with_vsync(window, width, height, false);
	upload_surfaces(surfaces_pointer);
	upload_fonts(fonts_pointer);
}

renderer_sdl::~renderer_sdl(){
    this->close();
}
