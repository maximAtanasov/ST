/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#ifndef RENDER_SDL_DEF
#define RENDER_SDL_DEF


#include <SDL_ttf.h>
#include <ST_util/bytell_hash_map.hpp>
#include <string>
#include <stdexcept>

///The renderer for the engine.
/**
 * This renderer is based entirely on the SDL2 renderer and uses it, along with some helper methods and
 * rendering techniques to draw textures, text and primitives to the screen.
 */

namespace ST::renderer_sdl {

    void set_draw_color(uint8_t, uint8_t, uint8_t, uint8_t);

    void clear_screen(SDL_Color color);

    void clear_screen();

    void present();

    void draw_background(uint16_t arg);

    void draw_background_parallax(uint16_t arg, uint16_t offset);

    void draw_overlay(uint16_t arg, uint8_t sprite, uint8_t sprite_num);

    void draw_texture(uint16_t arg, int32_t x, int32_t y);

    void draw_texture_scaled(uint16_t arg, int32_t x, int32_t y, float scale_x, float scale_y);

    void draw_rectangle(int32_t x, int32_t y, int32_t w, int32_t h, SDL_Color color);

    void draw_rectangle_filled(int32_t x, int32_t y, int32_t w, int32_t h, SDL_Color color);

    void draw_sprite(uint16_t arg, int32_t x, int32_t y, uint8_t sprite, uint8_t animation, uint8_t animation_num,
                     uint8_t sprite_num);

    void
    draw_sprite_scaled(uint16_t arg, int32_t x, int32_t y, uint8_t sprite, uint8_t animation, uint8_t animation_num,
                       uint8_t sprite_num, float scale_x, float scale_y);

    uint16_t draw_text_cached_glyphs(uint16_t font, const std::string &text, int x, int y, SDL_Color color_font);

    uint16_t draw_text_lru_cached(uint16_t font, const std::string &arg2, int x, int y, SDL_Color color_font);

    void upload_surfaces(ska::bytell_hash_map<uint16_t, SDL_Surface *> *surfaces);

    void upload_fonts(ska::bytell_hash_map<uint16_t, TTF_Font *> *fonts);

    void vsync_on();

    void vsync_off();

    int8_t initialize(SDL_Window *win, int16_t width, int16_t height);

    void close();

    void set_resolution(int16_t r_width, int16_t r_height);
}

#endif //RENDER_SDL_DEF
