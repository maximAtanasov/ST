/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 *
 */

#include "font_cache.hpp"
#include "texture.hpp"
#include <renderer_sdl.hpp>

namespace ST::renderer_sdl {
    void cache_font(TTF_Font *Font, uint16_t font_and_size);
    void process_surfaces(std::vector<std::pair<uint16_t, SDL_Surface *>> &surfaces_pairs);
}

static const uint16_t ATLAS_SIZE = 4096;
static const uint16_t atlas_grid_size = ATLAS_SIZE / 32;

#ifdef TESTING
SDL_Renderer *sdl_renderer;
#else
static SDL_Renderer *sdl_renderer;
#endif

//reference to a window
static SDL_Window *window;

//height and width of the renderer
static int16_t width;
static int16_t height;

//Textures with no corresponding surface in our assets need to be freed
#ifdef TESTING
ska::bytell_hash_map<uint16_t, ST::renderer_sdl::texture> textures{};
#else
static ska::bytell_hash_map<uint16_t, ST::renderer_sdl::texture> textures{};
#endif

static ska::bytell_hash_map<uint16_t, SDL_Surface *> *surfaces_pointer;
static ska::bytell_hash_map<uint16_t, TTF_Font *> *fonts_pointer;


//the fonts in this table do not need to be cleaned - these are just pointer to Fonts stored in the asset_manager and
//that will handle the cleanup
static ska::bytell_hash_map<uint16_t, TTF_Font *> fonts{};

//we do however need to clean up the cache as that lives on the GPU
static ska::bytell_hash_map<uint16_t, std::vector<ST::renderer_sdl::texture>> fonts_cache{};

static bool vsync = false;

static bool singleton_initialized = false;

/**
 * Initializes the renderer.
 * @param window The window to bind this renderer to.
 * @param width The virtual width of the window.
 * @param height The virtual height of the window.
 * @return Always 0.
 */
int8_t ST::renderer_sdl::initialize(SDL_Window* r_window, int16_t r_width, int16_t r_height){
    font_cache::set_max(100);

    if(singleton_initialized){
        throw std::runtime_error("The renderer cannot be initialized more than once!");
    }else{
        singleton_initialized = true;
    }

    //initialize renderer
	window = r_window;
	width = r_width;
	height = r_height;
    if(vsync){
        sdl_renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);
    }else{
        sdl_renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    }
    SDL_RenderSetLogicalSize(sdl_renderer, width, height);
    SDL_SetRenderDrawBlendMode(sdl_renderer, SDL_BLENDMODE_BLEND);
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "2" ); //Linear texture filtering
    set_draw_color(0, 0, 0, 255);
    return 0;
}

/**
 * Closes the renderer. Deletes all textures and fonts.
 */
void ST::renderer_sdl::close(){
    for (auto& it : fonts) {
        if (it.second != nullptr) {
            if (fonts[it.first] != nullptr) {
                for(auto& k : fonts_cache[it.first]) {
                    if(k.atlas != nullptr) {
                        SDL_DestroyTexture(k.atlas);
                        k.atlas = nullptr;
                    }
                }
                fonts[it.first] = nullptr;
            }
            it.second = nullptr;
        }
    }
    for ( auto& it : textures){
        if(it.second.atlas != nullptr ){
            SDL_DestroyTexture(it.second.atlas);
            it.second.atlas = nullptr;
        }
    }
    font_cache::clear();
    font_cache::close();
    SDL_DestroyRenderer(sdl_renderer);
    sdl_renderer = nullptr;
    singleton_initialized = false;
}

/**
 * Text rendering method for non-ASCII text -- works with cyrillic, expanded Latin (Spanish, German, etc..) and I guess all of UTF8
 * @param arg The font to render with.
 * @param arg2 The text to render.
 * @param x The x position to render at.
 * @param y The y position to render at.
 * @param color_font The color to render with.
 * @param size The size of the font to render at.
 * @return The width of the rendered string in pixels
 *
 * Note that the font must previously be loaded at the selected size.
 */
uint16_t ST::renderer_sdl::draw_text_lru_cached(uint16_t font, const std::string& arg2, int x, int y, SDL_Color color_font){
    TTF_Font* _font = fonts[font];
    int32_t texW = 0;
    if(_font != nullptr) [[likely]] {
        int32_t texH;
        SDL_Texture* cached_texture = font_cache::get_cached_string(arg2, font);
        if(cached_texture != nullptr){ //if the given string (with same size and font) is already cached, get it from cache
            SDL_QueryTexture(cached_texture, nullptr, nullptr, &texW, &texH);
            SDL_Rect Rect = {x, y - texH, texW, texH};
            SDL_SetTextureColorMod(cached_texture, color_font.r, color_font.g, color_font.b);
            SDL_RenderCopy(sdl_renderer, cached_texture, nullptr, &Rect);
        }else{ //else create a texture, render it, and then cache it - this is costly, so pick a good cache size
            SDL_Surface* text = TTF_RenderUTF8_Blended(_font, arg2.c_str(), color_font);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(sdl_renderer, text);
            SDL_QueryTexture(texture, nullptr, nullptr, &texW, &texH);
            SDL_Rect Rect = {x, y - texH, texW, texH};
            SDL_SetTextureColorMod(texture, color_font.r, color_font.g, color_font.b);
            SDL_RenderCopy(sdl_renderer, texture, nullptr, &Rect);
            SDL_FreeSurface(text);
            font_cache::cache_string(arg2, texture, font);
        }
    }
    return static_cast<uint16_t>(texW);
}

/**
 * This will draw text using cached glyphs - the fastest way possible, works only with ASCII and relatively simple fonts
 * (any complex cursive fonts won't be rendered properly).
 * @param arg The font to render with.
 * @param text The text to render.
 * @param x The x position to render at.
 * @param y The y position to render at.
 * @param color_font The color to render with.
 * @param size The size of the font to render at.
 * @return The width of the rendered string in pixels
 *
 * Note that the font must previously be loaded at the selected size.
 */
uint16_t ST::renderer_sdl::draw_text_cached_glyphs(uint16_t font, const std::string& text, const int x, const int y, const SDL_Color color_font) {
    int32_t tempX = 0;
    auto cached_vector = fonts_cache.find(font);
    if(cached_vector != fonts_cache.end()) [[likely]] {
        std::vector<ST::renderer_sdl::texture> tempVector = cached_vector->second;
        if(!tempVector.empty()) [[likely]] {
            tempX = x;
            const char* arg3 = text.c_str();
            for(int j = 0; arg3[j] != 0; j++) {
                ST::renderer_sdl::texture glyph = tempVector.at(static_cast<unsigned int>(arg3[j]-32));
                SDL_Texture* texture = glyph.atlas;
                SDL_Rect dstRect = {tempX, y - glyph.height, glyph.width, glyph.height};
                SDL_Rect srcRect = {glyph.atlas_h_offset, glyph.atlas_v_offset, glyph.width, glyph.height};
                SDL_SetTextureColorMod(texture, color_font.r, color_font.g, color_font.b);
                SDL_RenderCopy(sdl_renderer, texture, &srcRect, &dstRect);
                tempX += glyph.width;
            }
        }
    }
    return static_cast<uint16_t>(tempX - x);
}

/**
 * Upload all surface to the GPU. (Create textures from them).
 * @param surfaces The surfaces to upload.
 */
void ST::renderer_sdl::upload_surfaces(ska::bytell_hash_map<uint16_t, SDL_Surface*>* surfaces){
	if(surfaces != nullptr){
		surfaces_pointer = surfaces;
        //Clear all textures, when adding surfaces. This is far from optimal and additional logic should be implemented to check which atlases are to be cleared and
        //re-created.
        for (auto& it : textures){
            if(it.second.atlas != nullptr ){
                SDL_DestroyTexture(it.second.atlas);
                it.second.atlas = nullptr;
            }
        }
        std::vector<std::pair<uint16_t, SDL_Surface*>> surfaces_pairs{};
        for ( auto& it : *surfaces){
            surfaces_pairs.emplace_back(it.first, it.second);
        }
        process_surfaces(surfaces_pairs);
    }
}

//Helper function for sorting surfaces by their width and height.
bool sort_by_surface_width_height(const std::pair<uint16_t, SDL_Surface*> &a, const std::pair<uint16_t, SDL_Surface*> &b) {
    if(a.second-> w == b.second->w) {
        return a.second->h > b.second->h;
    }
    return a.second->w > b.second->w;
}

/**
 * Iterates over all surfaces, adding them to texture atlases if they have dimensions that are a power of 2.
 * If a surfaces how non POW2 dimensions, a separate texture is created for it. If the atlas becomes full,
 * this function is called recursively with the remaining textures.
 * @param surfaces_pairs Surfaces to process.
 */
void ST::renderer_sdl::process_surfaces(std::vector<std::pair<uint16_t, SDL_Surface *>> &surfaces_pairs) {
    if(surfaces_pairs.empty()) {
        return;
    } else if(surfaces_pairs.size() == 1) { //Only one surface in vector => no point in creating an atlas.
        auto tex = ST::renderer_sdl::texture{};
        tex.width = surfaces_pairs.back().second->w;
        tex.height = surfaces_pairs.back().second->h;
        tex.atlas_v_offset = 0;
        tex.atlas_h_offset = 0;
        tex.atlas = SDL_CreateTextureFromSurface(sdl_renderer, surfaces_pairs.back().second);
        textures[surfaces_pairs.back().first] = tex;
        return;
    }
    sort(surfaces_pairs.begin(), surfaces_pairs.end(), sort_by_surface_width_height);

    std::vector<std::pair<uint16_t, SDL_Surface*>> remaining_surface_pairs{};
    uint8_t atlas[atlas_grid_size][atlas_grid_size] = {0};
    int h_index = 0;
    int v_index = 0;

    SDL_Texture* atlas_texture = SDL_CreateTexture(sdl_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, ATLAS_SIZE, ATLAS_SIZE);
    SDL_SetTextureBlendMode(atlas_texture, SDL_BLENDMODE_BLEND);
    for(auto& it : surfaces_pairs) {
        int t_width = it.second->w/32;
        int t_height = it.second->h/32;

        //Texture either too large for atlas or has dimensions not power of 2
        if((it.second->w & (it.second->w - 1)) != 0 || (it.second->h & (it.second->h - 1)) != 0 || t_width > atlas_grid_size || t_height > atlas_grid_size) {
            auto tex = ST::renderer_sdl::texture{};
            tex.width = it.second->w;
            tex.height = it.second->h;
            tex.atlas_v_offset = 0;
            tex.atlas_h_offset = 0;
            tex.atlas = SDL_CreateTextureFromSurface(sdl_renderer, it.second);
            textures[it.first] = tex;
            continue;
        }

        while(atlas[h_index][v_index] != 0) {
            ++h_index;
            if(h_index == atlas_grid_size) {
                h_index = 0;
                ++v_index;

                if(v_index == atlas_grid_size) {
                    remaining_surface_pairs.emplace_back(it);
                    break;
                }
            }
        }

        if(v_index + t_height > atlas_grid_size) {
            remaining_surface_pairs.emplace_back(it);
            continue;
        }

        for(int i = h_index; i < h_index + t_width; ++i) {
            for(int j = v_index; j < v_index + t_height; ++j){
                atlas[i][j] = 1;
            }
        }

        //Blit to the larger surface
        auto tex = ST::renderer_sdl::texture{};
        tex.width = it.second->w;
        tex.height = it.second->h;
        tex.atlas_h_offset = h_index * 32;
        tex.atlas_v_offset = v_index * 32;
        tex.atlas = atlas_texture;
        textures[it.first] = tex;

        SDL_Rect dst_rect = {tex.atlas_h_offset, tex.atlas_v_offset, tex.width, tex.height};
        SDL_UpdateTexture(atlas_texture, &dst_rect, it.second->pixels, it.second->pitch);

        h_index += t_width;
        if(h_index == atlas_grid_size) {
            h_index = 0;
            v_index += 1;
        }
    }
    ST::renderer_sdl::process_surfaces(remaining_surface_pairs);
}

/**
 * Upload fonts to the GPU. (save and cache their glyphs).
 */
void ST::renderer_sdl::upload_fonts(ska::bytell_hash_map<uint16_t, TTF_Font*>* fonts_t) {
    if(fonts_t != nullptr) {
		fonts_pointer = fonts_t;
        for ( auto& it : *fonts_t) {
            if(it.second == nullptr) {
                for(auto& k : fonts_cache[it.first]) {
                    if(k.atlas != nullptr) {
                        SDL_DestroyTexture(k.atlas);
                        k.atlas = nullptr;
                    }
                }
                fonts[it.first] = nullptr;
            }
            else if(it.second != nullptr) {
                if(fonts[it.first] != nullptr) {
                    for(auto& k : fonts_cache[it.first]) {
                        if(k.atlas != nullptr) {
                            SDL_DestroyTexture(k.atlas);
                            k.atlas = nullptr;
                        }
                    }
                    fonts[it.first] = nullptr;
                }
                fonts[it.first] = it.second;
                cache_font(fonts[it.first], it.first);
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
void ST::renderer_sdl::cache_font(TTF_Font* Font, uint16_t font_and_size) {
    SDL_Color color_font = {255, 255, 255, 255};
    char temp[2];
    temp[1] = 0;
    std::vector<ST::renderer_sdl::texture> result_glyphs{};
    std::vector<SDL_Surface*> glyph_surfaces{};
    result_glyphs.reserve(95);
    glyph_surfaces.reserve(95);
    int32_t total_width = 0;
    int32_t max_height = 0;
    for(char j = 32; j < 127; j++) {
        temp[0] = j;
        SDL_Surface* glyph_surface = TTF_RenderUTF8_Blended(Font, temp, color_font);
        total_width += glyph_surface->w;
        if(glyph_surface->h > max_height) {
            max_height = glyph_surface->h;
        }
        glyph_surfaces.emplace_back(glyph_surface);
    }
    SDL_Texture* atlas_texture = SDL_CreateTexture(sdl_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, total_width, max_height);
    SDL_SetTextureBlendMode(atlas_texture, SDL_BLENDMODE_BLEND);
    int atlas_offset = 0;
    for(auto surface : glyph_surfaces) {
        ST::renderer_sdl::texture glyph{};
        glyph.width = surface->w;
        glyph.height = surface->h;
        glyph.atlas_h_offset = atlas_offset;
        glyph.atlas_v_offset = 0;
        glyph.atlas = atlas_texture;
        result_glyphs.push_back(glyph);
        SDL_Rect dst_rect = {glyph.atlas_h_offset, glyph.atlas_v_offset, glyph.width, glyph.height};
        SDL_UpdateTexture(atlas_texture, &dst_rect, surface->pixels, surface->pitch);
        SDL_FreeSurface(surface);
        atlas_offset += glyph.width;
    }
    fonts_cache[font_and_size] = result_glyphs;
}

/**
 * Turns on vsync.
 */
void ST::renderer_sdl::vsync_on(){
    SDL_RendererInfo info;
    SDL_GetRendererInfo(sdl_renderer, &info);
    if(!(info.flags & SDL_RENDERER_PRESENTVSYNC)){ // NOLINT(hicpp-signed-bitwise)
        vsync = true;
        close();
        initialize(window, width, height);
        upload_surfaces(surfaces_pointer);
        upload_fonts(fonts_pointer);
    }
}

/**
 * Turns off vsync.
 */
void ST::renderer_sdl::vsync_off(){
    SDL_RendererInfo info;
    SDL_GetRendererInfo(sdl_renderer, &info);
    if(info.flags & SDL_RENDERER_PRESENTVSYNC){ // NOLINT(hicpp-signed-bitwise)
        vsync = false;
        close();
        initialize(window, width, height);
        upload_surfaces(surfaces_pointer);
        upload_fonts(fonts_pointer);
    }
}

//INLINED METHODS

/**
 * Draw a texture at a given position.
 * @param arg The hash of the texture name.
 * @param x The X position to render at.
 * @param y The Y position to render at.
 */
void ST::renderer_sdl::draw_texture(const uint16_t arg, int32_t x, int32_t y) {
    auto data = textures.find(arg);
    if(data != textures.end()) {
        auto texture = data->second;
        SDL_Rect src_rect = {texture.atlas_h_offset, texture.atlas_v_offset, texture.width, texture.height};
        SDL_Rect dst_rect = {x, y - texture.height, texture.width, texture.height};
        SDL_RenderCopy(sdl_renderer, texture.atlas, &src_rect, &dst_rect);
    }
}

/**
 * Draw a texture at a given position and scale it.
 * @param arg The hash of the texture name.
 * @param x The X position to render at.
 * @param y The Y position to render at.
 */
void ST::renderer_sdl::draw_texture_scaled(const uint16_t arg, int32_t x, int32_t y, float scale_x, float scale_y) {
    auto data = textures.find(arg);
    if(data != textures.end()) {
        auto texture = data->second;
        SDL_Rect src_rect = {texture.atlas_h_offset, texture.atlas_v_offset, texture.width, texture.height};
        SDL_Rect dst_rect = {x,
                             y - static_cast<int>(static_cast<float>(texture.height) * scale_y),
                             static_cast<int>(static_cast<float>(texture.width) * scale_x),
                             static_cast<int>(static_cast<float>(texture.height) * scale_y)};

        SDL_RenderCopy(sdl_renderer, texture.atlas, &src_rect, &dst_rect);
    }
}

/**
 * Draws a filled rectangle on the screen.
 * @param x The X position to draw at.
 * @param y The Y position to draw at.
 * @param w The width of the rectangle.
 * @param h The height of the rectangle.
 * @param color The color of the rectangle.
 */
void ST::renderer_sdl::draw_rectangle_filled(int32_t x, int32_t y, int32_t w, int32_t h, SDL_Color color) {
    SDL_Rect Rect = {x, y, w, h};
    SDL_SetRenderDrawColor(sdl_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(sdl_renderer, &Rect);
    SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 255);
}

/**
 * Draws a rectangle on the screen.
 * @param x The X position to draw at.
 * @param y The Y position to draw at.
 * @param w The width of the rectangle.
 * @param h The height of the rectangle.
 * @param color The color of the rectangle.
 */
void ST::renderer_sdl::draw_rectangle(int32_t x, int32_t y, int32_t w, int32_t h, SDL_Color color) {
    SDL_Rect Rect = {x, y, w, h};
    SDL_SetRenderDrawColor(sdl_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawRect(sdl_renderer, &Rect);
    SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 255);
}

/**
 * Draws a texture that fills the entire screen (Background).
 * @param arg The hash of the texture name.
 */
void ST::renderer_sdl::draw_background(const uint16_t arg) {
    auto data = textures.find(arg);
    if(data != textures.end()) {
        auto texture = data->second;
        SDL_Rect src_rect = {texture.atlas_h_offset, texture.atlas_v_offset, texture.width, texture.height};
        SDL_RenderCopy(sdl_renderer, texture.atlas, &src_rect, nullptr);
    }
}

/**
 * @param arg The hash of the name of the texture.
 * @param offset The offset in the texture for the parallax effect
 */
void ST::renderer_sdl::draw_background_parallax(const uint16_t arg, const uint16_t offset) {
    auto data = textures.find(arg);
    if(data != textures.end()) {
        auto texture = data->second;

        float bg_ratio = (float)texture.width/(float)width;
        int src_offset = (int)((float)offset*bg_ratio);

        SDL_Rect dst_rect1 = {0, 0, width - offset, height};
        SDL_Rect src_rect1 = {texture.atlas_h_offset + src_offset, texture.atlas_v_offset, texture.width - src_offset, texture.height};
        SDL_Rect src_rect2 = {texture.atlas_h_offset, texture.atlas_v_offset, src_offset, texture.height};
        SDL_Rect dst_rect2 = {width - offset, 0, offset, height};

        SDL_RenderCopy(sdl_renderer, texture.atlas, &src_rect1, &dst_rect1);
        SDL_RenderCopy(sdl_renderer, texture.atlas, &src_rect2, &dst_rect2);
    }
}

/**
 * Draws a texture that is a spritesheet.
 * @param arg The hash of the name of the spritesheet.
 * @param x The X position to render at.
 * @param y The Y position to render at.
 * @param sprite The number of the sprite in the texture. (Column in the spritesheet).
 * @param animation The number of the animation in the texture (Row in the spritesheet).
 * @param animation_num The total number of animations in a spritesheet (Rows in the spritesheet).
 * @param sprite_num The total number of sprites in a spritesheet. (Columns in a spritesheet).
 */
void ST::renderer_sdl::draw_sprite(uint16_t arg, int32_t x, int32_t y, uint8_t sprite, uint8_t animation, uint8_t animation_num, uint8_t sprite_num) {
    auto data = textures.find(arg);
    if(data != textures.end()) {
        auto texture = data->second;

        int temp1 = texture.height / animation_num;
        int temp2 = texture.width / sprite_num;
        SDL_Rect dst_rect = {x, y - temp1, temp2, temp1};
        SDL_Rect src_rect = {texture.atlas_h_offset + (sprite * temp2), texture.atlas_v_offset + (temp1 * (animation - 1)), temp2, temp1};
        SDL_RenderCopy(sdl_renderer, texture.atlas, &src_rect, &dst_rect);
    }
}

/**
 * Draws a texture that is a spritesheet.
 * @param arg The hash of the name of the spritesheet.
 * @param x The X position to render at.
 * @param y The Y position to render at.
 * @param sprite The number of the sprite in the texture. (Column in the spritesheet).
 * @param animation The number of the animation in the texture (Row in the spritesheet).
 * @param animation_num The total number of animations in a spritesheet (Rows in the spritesheet).
 * @param sprite_num The total number of sprites in a spritesheet. (Columns in a spritesheet).
 */
void ST::renderer_sdl::draw_sprite_scaled(uint16_t arg, int32_t x, int32_t y, uint8_t sprite, uint8_t animation, uint8_t animation_num, uint8_t sprite_num, float scale_x, float scale_y) {
    auto data = textures.find(arg);
    if(data != textures.end()) {
        auto texture = data->second;

        int temp1 = texture.height / animation_num;
        int temp2 = texture.width / sprite_num;
        SDL_Rect dst_rect = {x,
                             y - static_cast<int>(static_cast<float>(temp1) * scale_y),
                             static_cast<int>(static_cast<float>(temp2) * scale_x),
                             static_cast<int>(static_cast<float>(temp1) * scale_y)};
        SDL_Rect src_rect = {texture.atlas_h_offset + (sprite * temp2), texture.atlas_v_offset + (temp1 * (animation - 1)), temp2, temp1};
        SDL_RenderCopy(sdl_renderer, texture.atlas, &src_rect, &dst_rect);
    }
}

/**
 * Draws an animated overlay.
 * Works similarly to draw_sprite, except only one animation is supported.
 * @param arg The hash of the texture name.
 * @param sprite The number of the sprite to use.
 * @param sprite_num The total number of frames this spritesheet has.
 */
void ST::renderer_sdl::draw_overlay(uint16_t arg, uint8_t sprite, uint8_t sprite_num) {
    auto data = textures.find(arg);
    if(data != textures.end()) {
        auto texture = data->second;

        SDL_Rect src_rect = {sprite * (texture.width / sprite_num), 0, texture.width / sprite_num, texture.height};
        SDL_RenderCopy(sdl_renderer, texture.atlas, &src_rect, nullptr);
    }
}

/**
 * Clears the screen given a color
 */
void ST::renderer_sdl::clear_screen(SDL_Color color) {
    SDL_SetRenderDrawColor(sdl_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(sdl_renderer);
    SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 0);
}

/**
 * Clears the screen with the color set by set_draw_color
 */
void ST::renderer_sdl::clear_screen() {
    SDL_RenderClear(sdl_renderer);
}

/**
 * Sets a draw color.
 * @param r Red value.
 * @param g Green value.
 * @param b Blue value.
 * @param a Alpha value.
 */
void ST::renderer_sdl::set_draw_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_SetRenderDrawColor(sdl_renderer, r, g, b, a);
}

/**
 * Presents the framebuffer to the window.
 */
void ST::renderer_sdl::present() {
    SDL_RenderPresent(sdl_renderer);
}

/**
 * Sets the internal rendering resolution
 * @param r_width Width
 * @param r_height Height
 */
void ST::renderer_sdl::set_resolution(int16_t r_width, int16_t r_height) {
    width = r_width;
    height = r_height;
    SDL_RenderSetLogicalSize(sdl_renderer, width, height);
}


