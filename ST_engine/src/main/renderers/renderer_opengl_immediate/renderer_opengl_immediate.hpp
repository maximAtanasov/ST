#ifndef RENDER_GL_LEGACY_DEF
#define RENDER_GL_LEGACY_DEF

#include "../renderer.hpp"
#include <unordered_map>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "font_cache_opengl.hpp"
#include "GL/glew.h"
#include "SDL2/SDL_opengl.h"
#include "texture_opengl_immediate.hpp"


class renderer_opengl_immediate : public virtual renderer{

private:
	int w_width, w_height;
    SDL_Window* window;
    SDL_GLContext gl_contex;
    GLuint texture_ids = 0;
    font_cache_opengl* gFont_cache;
    std::unordered_map<size_t, texture_gl_legacy*> textures;
    std::unordered_map<std::string, TTF_Font*> fonts;
    std::unordered_map<std::string, std::vector<texture_gl_legacy*>> fonts_cache;
    void cache_font(TTF_Font* Font, std::string font_and_size);
    void draw_text_normal(std::string, std::string, int, int, SDL_Color, int);
    void draw_text_cached(std::string, std::string, int, int, SDL_Color, int);
    texture_gl_legacy* create_texture_from_surface(SDL_Surface* arg);
    void draw_texture_(texture_gl_legacy* Texture, int x, int y); //does the actual drawing
    GLuint new_texture_id();

public:
    void set_draw_color(Uint8,Uint8,Uint8,Uint8) override;
    void clear_screen() override;
    void present() override;
    void draw_background(size_t) override;
    void draw_overlay(size_t arg, int sprite, int sprite_num) override;
    void draw_texture(size_t, int, int) override;
    void draw_rectangle(int, int, int, int, SDL_Color) override;
    void draw_rectangle_filled(int, int, int, int, SDL_Color) override;
    void draw_sprite(size_t, int, int, int, int, int, int) override;
    void draw_text(std::string, std::string, int, int, SDL_Color, int, int) override;
    void upload_surfaces(std::unordered_map<size_t, SDL_Surface*>* surfaces) override;
    void upload_fonts(std::unordered_map<std::string, TTF_Font*>* fonts) override;
	void vsync_on() override;
    void vsync_off() override;
    int initialize(SDL_Window* win, int width, int height) override;
    void close() override;
    ~renderer_opengl_immediate() override;
};

#endif
