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


#include <defs.hpp>
#include <renderer/renderer_sdl/font_cache.hpp>
#include <ST_util/bytell_hash_map.hpp>

///The renderer for the engine.
/**
 * This renderer is based entirely on the SDL2 renderer and uses it, along with some helper methods and
 * rendering techniques to draw textures, text and primitives to the screen.
 */
class renderer_sdl{
private:
	friend class renderer_sdl_tests;

    SDL_Renderer* sdl_renderer;
    font_cache gFont_cache;

	//reference to a window
	SDL_Window* window;

	//height and width of the renderer
	int16_t width;
	int16_t height;

    //Textures with no corresponding surface in our assets need to be freed
    ska::bytell_hash_map<size_t, SDL_Texture*> textures;

	ska::bytell_hash_map<size_t, SDL_Surface*>* surfaces_pointer;
	ska::bytell_hash_map<std::string, TTF_Font*>* fonts_pointer;


    //the fonts in this table do not need to be cleaned - these are just pointer to Fonts stored in the asset_manager and
    //that will handle the cleanup
    ska::bytell_hash_map<std::string, TTF_Font*> fonts;

    //we do however need to cleanup the cache as that lives on the GPU
    ska::bytell_hash_map<std::string, std::vector<SDL_Texture*>> fonts_cache;

	bool vsync = false;

	void cache_font(TTF_Font* Font, std::string font_and_size);
    void draw_text_lru_cached(std::string, std::string, int, int, SDL_Color, int);
    void draw_text_cached_glyphs(std::string, std::string, int, int, SDL_Color, int) const;
	int initialize_with_vsync(SDL_Window* window, int16_t width, int16_t height, bool vsync);

public:
    void set_draw_color(uint8_t,uint8_t,uint8_t,uint8_t) ;
    void clear_screen() const;
    void present() const;
    void draw_background(size_t arg) const;
    void draw_overlay(size_t arg, int sprite, int sprite_num) const;
    void draw_texture(size_t arg, int x, int y) const;
    void draw_rectangle(int x, int y, int w, int h, SDL_Color color) const;
    void draw_rectangle_filled(int x, int y, int w, int h, SDL_Color color) const;
    void draw_sprite(size_t arg, int x, int y, int sprite, int animation, int animation_num, int sprite_num) const;
    void draw_text(std::string arg, std::string arg2, int x, int y, SDL_Color color_font , int size, int flag);
    void upload_surfaces(ska::bytell_hash_map<size_t, SDL_Surface*>* surfaces);
    void upload_fonts(ska::bytell_hash_map<std::string, TTF_Font*>* fonts);
    void vsync_on();
    void vsync_off();
    int initialize(SDL_Window* win, int16_t width, int16_t height);
    void close();
    ~renderer_sdl();
	void set_resolution(int16_t width, int16_t height);
};

//INLINED METHODS

/**
 * Draw a texture at a given position.
 * @param arg The hash of the texture name.
 * @param x The X position to render at.
 * @param y The Y position to render at.
 */
inline void renderer_sdl::draw_texture(const size_t arg, int x, int y)  const{
    auto texture = textures.find(arg);
	if (texture != textures.end()) {
		int tex_w, tex_h;
		SDL_QueryTexture(texture->second, nullptr, nullptr, &tex_w, &tex_h);
		SDL_Rect src_rect = {x, y - tex_h, tex_w, tex_h};
		SDL_RenderCopy(sdl_renderer, texture->second, nullptr, &src_rect);
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
inline void renderer_sdl::draw_rectangle_filled(int x, int y, int w, int h, SDL_Color color) const{
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
inline void renderer_sdl::draw_rectangle(int x, int y, int w, int h, SDL_Color color) const{
	SDL_Rect Rect = {x, y, w, h};
	SDL_SetRenderDrawColor(sdl_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawRect(sdl_renderer, &Rect);
	SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 255);
}

/**
 * Draws a texture that fills the entire screen (Background).
 * @param arg The hash of the texture name.
 */
inline void renderer_sdl::draw_background(const size_t arg) const{
	auto texture = textures.find(arg);
	if(texture != textures.end()){
		SDL_RenderCopy(sdl_renderer, texture->second, nullptr, nullptr);
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
inline void renderer_sdl::draw_sprite(size_t arg, int x, int y, int sprite, int animation, int animation_num, int sprite_num) const{
	auto texture = textures.find(arg);
	if(texture != textures.end()){
		int tex_w, tex_h;
		SDL_QueryTexture(texture->second, nullptr, nullptr, &tex_w, &tex_h);
		int temp1 = tex_h/animation_num;
		int temp2 = tex_w/sprite_num;
		SDL_Rect dst_rect = {x, y - temp1, temp2, temp1};
		SDL_Rect src_rect = {sprite*(tex_w/sprite_num), temp1*(animation-1), temp2, temp1};
		SDL_RenderCopy(sdl_renderer, texture->second, &src_rect, &dst_rect);
	}
}

/**
 * Draws an animated overlay.
 * Works similary to draw_sprite, except only one animation is supported.
 * @param arg The hash of the texture name.
 * @param sprite The number of the sprite to use.
 * @param sprite_num The total number of frames this spritesheet has.
 */
inline void renderer_sdl::draw_overlay(size_t arg, int sprite, int sprite_num) const{
	int animation_num = 1;
	int animation = 1;
	auto texture = textures.find(arg);
	if(texture != textures.end()){
		int tex_w, tex_h;
		SDL_QueryTexture(texture->second, nullptr, nullptr, &tex_w, &tex_h);
		int temp1 = tex_h/animation_num;
		int temp2 = tex_w/sprite_num;
		SDL_Rect src_rect = {sprite*(tex_w/sprite_num), temp1*(animation-1), temp2, temp1};
		SDL_RenderCopy(sdl_renderer, texture->second, &src_rect, nullptr);
	}
}

/**
 * Clears the screen.
 */
inline void renderer_sdl::clear_screen() const{
	SDL_RenderClear(sdl_renderer);
}

/**
 * Sets a draw color.
 * @param r Red value.
 * @param g Green value.
 * @param b Blue value.
 * @param a Alpha value.
 */
inline void renderer_sdl::set_draw_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
	SDL_SetRenderDrawColor(sdl_renderer, r, g, b, a);
}

/**
 * Presents the framebuffer to the window.
 */
inline void renderer_sdl::present() const{
	SDL_RenderPresent(sdl_renderer);
}

inline void renderer_sdl::set_resolution(int16_t width, int16_t height) {
    close();
	this->width = width;
	this->height = height;
	initialize_with_vsync(window, width, height, vsync);
    upload_surfaces(surfaces_pointer);
    upload_fonts(fonts_pointer);
}

#endif //RENDER_SDL_DEF
