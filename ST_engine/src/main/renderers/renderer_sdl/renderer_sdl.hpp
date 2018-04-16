/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#ifndef RENDER_SDL_DEF
#define RENDER_SDL_DEF

#include <unordered_map>
#include <defs.hpp>
#include <renderers/renderer_sdl/font_cache.hpp>
#include <renderers/renderer.hpp>

class renderer_sdl{
private:
    SDL_Renderer* sdl_renderer;
    font_cache* gFont_cache;

	//reference to a window
	SDL_Window* window;

	//height and width of the renderer
	int width;
	int height;

    //Textures with no corresponding surface in our assets need to be freed
    std::unordered_map<size_t, SDL_Texture*> textures;

	std::unordered_map<size_t, SDL_Surface*>* surfaces_pointer;
	std::unordered_map<std::string, TTF_Font*>* fonts_pointer;


    //the fonts in this table do not need to be cleaned - these are just pointer to Fonts stored in the asset_manager and
    //that will handle the cleanup
    std::unordered_map<std::string, TTF_Font*> fonts;

    //we do however need to cleanup the cache as that lives on the GPU
    std::unordered_map<std::string, std::vector<SDL_Texture*>> fonts_cache;

	//texture and surface to render lights to
	Uint32* pixels;
	SDL_Texture* lights_texture{};

    void cache_font(TTF_Font* Font, std::string font_and_size);
    void draw_text_normal(std::string, std::string, int, int, SDL_Color, int);
    void draw_text_cached(std::string, std::string, int, int, SDL_Color, int);
	int initialize_with_vsync(SDL_Window* win, int width, int height, bool vsync);

public:
	void draw_lights(Uint8 arg[1920][1080]);
    void set_draw_color(Uint8,Uint8,Uint8,Uint8) ;
    void clear_screen() ;
    void present() ;
    void draw_background(size_t);
    void draw_overlay(size_t arg, int sprite, int sprite_num);
    void draw_texture(size_t, int, int);
    void draw_rectangle(int, int, int, int, SDL_Color);
    void draw_rectangle_filled(int, int, int, int, SDL_Color);
    void draw_sprite(size_t, int, int, int, int, int, int);
    void draw_text(std::string, std::string, int, int, SDL_Color, int, int);
    void upload_surfaces(std::unordered_map<size_t, SDL_Surface*>* surfaces);
    void upload_fonts(std::unordered_map<std::string, TTF_Font*>* fonts);
    void vsync_on();
    void vsync_off();
    int initialize(SDL_Window* win, int width, int height);
    void close();
    ~renderer_sdl();
};

//INLINED METHODS

inline void renderer_sdl::draw_lights(Uint8 arg[1920][1080]){
	int k = 0;
	for(Uint16 i = 0; i < height; i++){
		for(Uint16 j = 0; j < width; j++){
			pixels[k] = arg[j][i] << (Uint8)24;
			k++;
		};
	}
	SDL_UpdateTexture(lights_texture, nullptr, pixels, width*sizeof(Uint32));
	SDL_RenderCopy(sdl_renderer, lights_texture, nullptr, nullptr);
}

inline void renderer_sdl::draw_texture(size_t arg, int x, int y) {
	SDL_Texture *temp = textures[arg];
	if (temp != nullptr) {
		int tex_w, tex_h;
		SDL_QueryTexture(temp, nullptr, nullptr, &tex_w, &tex_h);
		SDL_Rect src_rect = {x, y - tex_h, tex_w, tex_h};
		SDL_RenderCopy(sdl_renderer, temp, nullptr, &src_rect);
	}
}

inline void renderer_sdl::draw_rectangle_filled(int x, int y, int w, int h, SDL_Color color){
	SDL_Rect Rect = {x, y, w, h};
	SDL_SetRenderDrawColor(sdl_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(sdl_renderer, &Rect);
	SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 255);
}

inline void renderer_sdl::draw_rectangle(int x, int y, int w, int h, SDL_Color color){
	SDL_Rect Rect = {x, y, w, h};
	SDL_SetRenderDrawColor(sdl_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawRect(sdl_renderer, &Rect);
	SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 255);
}

inline void renderer_sdl::draw_background(size_t arg){
	SDL_Texture* temp = textures[arg];
	if(temp != nullptr){
		SDL_RenderCopy(sdl_renderer, temp, nullptr, nullptr);
	}
}

inline void renderer_sdl::draw_sprite(size_t arg, int x, int y, int sprite, int animation, int animation_num, int sprite_num){
	SDL_Texture* temp = textures[arg];
	if(temp != nullptr){
		int tex_w, tex_h;
		SDL_QueryTexture(temp, nullptr, nullptr, &tex_w, &tex_h);
		int temp1 = tex_h/animation_num;
		int temp2 = tex_w/sprite_num;
		SDL_Rect src_rect = {x, y - temp1, temp2, temp1};
		SDL_Rect dst_rect = {sprite*(tex_w/sprite_num), temp1*(animation-1), temp2, temp1};
		SDL_RenderCopy(sdl_renderer, temp, &dst_rect, &src_rect);
	}
}

inline void renderer_sdl::draw_overlay(size_t arg, int sprite, int sprite_num){
	int animation_num = 1;
	int animation = 1;
	SDL_Texture* temp = textures[arg];
	if(temp != nullptr){
		int tex_w, tex_h;
		SDL_QueryTexture(temp, nullptr, nullptr, &tex_w, &tex_h);
		int temp1 = tex_h/animation_num;
		int temp2 = tex_w/sprite_num;
		SDL_Rect dst_rect = {sprite*(tex_w/sprite_num), temp1*(animation-1), temp2, temp1};
		SDL_RenderCopy(sdl_renderer, temp, &dst_rect, nullptr);
	}
}

inline void renderer_sdl::clear_screen(){
	SDL_RenderClear(sdl_renderer);
}

inline void renderer_sdl::set_draw_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a){
	SDL_SetRenderDrawColor(sdl_renderer, r, g, b, a);
}

inline void renderer_sdl::present(){
	SDL_RenderPresent(sdl_renderer);
}

#endif
