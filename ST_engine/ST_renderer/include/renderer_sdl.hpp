/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#ifndef RENDER_SDL_DEF
#define RENDER_SDL_DEF


#include <SDL_ttf.h>
#include <ST_util/bytell_hash_map.hpp>

///The renderer for the engine.
/**
 * This renderer is based entirely on the SDL2 renderer and uses it, along with some helper methods and
 * rendering techniques to draw textures, text and primitives to the screen.
 */

namespace ST {
    namespace renderer_sdl {

        void set_draw_color(uint8_t, uint8_t, uint8_t, uint8_t);

        void clear_screen();

        void present();

        void draw_background(size_t arg);

        void draw_overlay(size_t arg, int sprite, int sprite_num);

        void draw_texture(size_t arg, int x, int y);

        void draw_rectangle(int x, int y, int w, int h, SDL_Color color);

        void draw_rectangle_filled(int x, int y, int w, int h, SDL_Color color);

        void draw_sprite(size_t arg, int x, int y, int sprite, int animation, int animation_num, int sprite_num);

        uint16_t draw_text(const std::string& arg, const std::string& arg2, int x, int y, SDL_Color color_font , uint8_t size, int flag);

        void upload_surfaces(ska::bytell_hash_map<size_t, SDL_Surface *> *surfaces);

        void upload_fonts(ska::bytell_hash_map<std::string, TTF_Font *> *fonts);

        void vsync_on();

        void vsync_off();

        int8_t initialize(SDL_Window *win, int16_t width, int16_t height);

        void close();

        void set_resolution(int16_t r_width, int16_t r_height);

    }
}

#endif //RENDER_SDL_DEF
