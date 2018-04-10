#ifndef RENDER_DEF
#define RENDER_DEF

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <unordered_map>

//Abstract interface that has to be implemented
class renderer{

public:
    virtual void set_draw_color(Uint8,Uint8,Uint8,Uint8) = 0;
    virtual void clear_screen() = 0;
    virtual void present() = 0;
    virtual void draw_background(size_t) = 0;
    virtual void draw_overlay(size_t arg, int sprite, int sprite_num) = 0;
    virtual void draw_texture(size_t, int, int) = 0;
    virtual void draw_rectangle(int, int, int, int, SDL_Color) = 0;
    virtual void draw_rectangle_filled(int, int, int, int, SDL_Color) = 0;
    virtual void draw_sprite(size_t, int, int, int, int, int, int) = 0;
    virtual void draw_text(std::string, std::string, int, int, SDL_Color, int, int) = 0;
    virtual void upload_surfaces(std::unordered_map<size_t, SDL_Surface*>* surfaces) = 0;
    virtual void upload_fonts(std::unordered_map<std::string, TTF_Font*>* fonts) = 0;
    virtual void vsync_on() = 0;
    virtual void vsync_off() = 0;
    virtual int initialize(SDL_Window* win, int width, int height) = 0;
    virtual void close() = 0;
    virtual ~renderer() = default;
};

#endif
