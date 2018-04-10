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
	void draw_lights(Uint8 arg[1080][1920], int w, int h);
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

#endif
