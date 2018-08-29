/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <renderer/renderer_sdl/renderer_sdl.hpp>

/**
 * Initializes the renderer.
 * @param window The window to bind this renderer to.
 * @param width The virtual width of the window.
 * @param height The virtual height of the window.
 * @return Always 0.
 */
int renderer_sdl::initialize(SDL_Window* window, int16_t width, int16_t height){
    gFont_cache.set_max(100);
    //initialize renderer
	this->window = window;
	this->width = width;
	this->height = height;
    sdl_renderer = SDL_CreateRenderer( this->window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(sdl_renderer, this->width, this->height);
    SDL_SetRenderDrawBlendMode(sdl_renderer, SDL_BLENDMODE_BLEND);
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ); //Linear texture filtering
    set_draw_color(0, 0, 0, 255);
    return 0;
}

/**
 * Initializes the renderer.
 * Same as the regular initialization method, except for an additional parameter that specify if VSYNC should be on or off.
 * @param window The window to bind this renderer to.
 * @param width The virtual width of the window.
 * @param height The virtual height of the window.
 * @param vsync True to enable VSYNC, false otherwise.
 * @return Always 0.
 */
int renderer_sdl::initialize_with_vsync(SDL_Window* window, int width, int height, bool vsync){
    gFont_cache.set_max(100);
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
    return 0;
}

/**
 * Closes the renderer. Deletes all textures and fonts.
 */
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
    gFont_cache.clear();
    SDL_DestroyRenderer(sdl_renderer);
    sdl_renderer = nullptr;
}

/**
 * Text rendering method for non-ASCII text -- works with cyrillic, expanded Latin (Spanish, German, etc..) and I guess all of UTF8
 * @param arg The font to render with.
 * @param arg2 The text to render.
 * @param x The x position to render at.
 * @param y The y position to render at.
 * @param color_font The color to render with.
 * @param size The size of the font to render at.
 *
 * Note that the font must previously be loaded at the selected size.
 */
void renderer_sdl::draw_text_lru_cached(std::string arg, std::string arg2, int x, int y, SDL_Color color_font, int size){
    std::string font_and_size = arg+std::to_string(size);
    TTF_Font* font = fonts[font_and_size];
    if(font != nullptr){
        int texW, texH;
        SDL_Texture* cached_texture = gFont_cache.get_cached_string(arg2, arg, size);
        if(cached_texture != nullptr){ //if the given string (with same size and font) is already cached, get it from cache
            SDL_QueryTexture(cached_texture, nullptr, nullptr, &texW, &texH);
            SDL_Rect Rect = {x, y, texW, texH};
            SDL_SetTextureColorMod(cached_texture, color_font.r, color_font.g, color_font.b);
            SDL_RenderCopy(sdl_renderer, cached_texture, nullptr, &Rect);
        }else{ //else create a texture, render it, and then cache it - this is costly, so pick a good cache size
            SDL_Surface* text = TTF_RenderUTF8_Blended(font, arg2.c_str(), color_font);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(sdl_renderer, text);
            SDL_QueryTexture(texture, nullptr, nullptr, &texW, &texH);
            SDL_Rect Rect = {x, y, texW, texH};
            SDL_SetTextureColorMod(texture, color_font.r, color_font.g, color_font.b);
            SDL_RenderCopy(sdl_renderer, texture, nullptr, &Rect);
            SDL_FreeSurface(text);
            gFont_cache.cache_string(arg2, texture, arg, size);
        }
    }
}

/**
 * This will draw text using cached glyphs - the fastest way possible, works only with ASCII and relatively simple fonts
 * (any complex cursive fonts won't be rendered properly).
 * @param arg The font to render with.
 * @param arg2 The text to render.
 * @param x The x position to render at.
 * @param y The y position to render at.
 * @param color_font The color to render with.
 * @param size The size of the font to render at.
 *
 * Note that the font must previously be loaded at the selected size.
 */
void renderer_sdl::draw_text_cached_glyphs(const std::string arg, const std::string arg2, const int x, const int y,
                                           const SDL_Color color_font, const int size) const{
    std::string font_and_size = arg+std::to_string(size);
    auto cached_vector = fonts_cache.find(font_and_size);
    if(cached_vector != fonts_cache.end()){
        std::vector<SDL_Texture*> tempVector = cached_vector->second;
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
}

/**
 * This method will decide which of the two text rendering methods to use based on the flag that is passed, if you are unsure if your
 * font will render properly just pass in 0 (the default for all non-internal engine text).
 * @param arg The font to render with.
 * @param arg2 The text to render.
 * @param x The x position to render at.
 * @param y The y position to render at.
 * @param color_font The color to render with.
 * @param size The size of the font to render at.
 * @param flag 1 to render with draw_text_cached, 0 to render with draw_text_normal
 * and any other value will cause a method to be picked automatically.
 *
 * Note that the font must previously be loaded at the selected size.
 */
void renderer_sdl::draw_text(std::string arg, std::string arg2, int x, int y, SDL_Color color_font , int size, int flag){
    if(flag == 1){
        draw_text_cached_glyphs(arg, arg2, x, y, color_font, size);
    }else if(flag == 0){
        draw_text_lru_cached(arg, arg2, x, y, color_font, size);
    }else{
        for(unsigned int i = 0; i < arg2.size(); i++) {
            if (arg2.at(i) > 126 || (arg2.at(i) < 32)) {
                draw_text_lru_cached(arg, arg2, x, y, color_font, size);
                return;
            }
        }
        draw_text_cached_glyphs(arg, arg2, x, y, color_font, size);
    }
}

/**
 * Upload all surface to the GPU. (Create textures from them).
 * @param surfaces The surfaces to upload.
 */
void renderer_sdl::upload_surfaces(ska::bytell_hash_map<size_t, SDL_Surface*>* surfaces){
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

/**
 * Upload fonts to the GPU. (save and cache their glyphs).
 */
void renderer_sdl::upload_fonts(ska::bytell_hash_map<std::string, TTF_Font*>* fonts){
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

/**
 * Caches all glyphs of a font at a given size.
 * Works with the draw_text_cached method.
 * Do not confuse this method with the font_cache class, they have nothing in common, this caches fonts to be used
 * with ASCII strings, font_cache is a LRU cache that works with any string.
 * @param Font The Font to render with.
 * @param font_and_size The name+size of the font.
 */
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

/**
 * Turns on vsync.
 */
void renderer_sdl::vsync_on(){
	close();
	initialize_with_vsync(window, width, height, true);
	upload_surfaces(surfaces_pointer);
	upload_fonts(fonts_pointer);
}

/**
 * Turns off vsync.
 */
void renderer_sdl::vsync_off(){
	close();
	initialize_with_vsync(window, width, height, false);
	upload_surfaces(surfaces_pointer);
	upload_fonts(fonts_pointer);
}

/**
 * Destroys this renderer object.
 */
renderer_sdl::~renderer_sdl(){
    this->close();
}
