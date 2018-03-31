#ifndef RENDER_GL
#define RENDER_GL

#include "defs.hpp"
#include <unordered_map>
#include <SDL2/SDL.h>
#include "SDL2/SDL_ttf.h"
#include <GL/glew.h>
#include "texture_opengl.hpp"
#include "vertex.hpp"
#include "renderer.hpp"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))


class renderer_opengl : public renderer{

private:
	GLuint atlases_used = 0;
    SDL_Window* window;
    GLuint id = 1; //texture ids
	GLuint VBOID;
	GLuint VAID;
	GLuint render_num = 0;
	int vbo_index = 0;

	//VBO ID and buffer
	GLuint main_vbo;
	GLuint main_uv;
	GLfloat vbo_buffer[24000];
	GLfloat uv_buffer[24000];

	std::vector<texture*> render_list;
	GLuint shader_id = 0;
	GLuint tex_unit = 0;
	GLuint vbo_offset = 0;
	GLuint atlas_array[8]; //Max of 8 atlases - support hardware with as little as 8 texture Units


    //font_cache_opengl* gFont_cache;
    std::unordered_map<int, texture*> textures;
    std::unordered_map<std::string, TTF_Font*> fonts;
    std::unordered_map<std::string, std::vector<texture*>> fonts_cache;
    void cache_font(TTF_Font* Font, std::string font_and_size);
    void draw_text_normal(std::string, std::string, int, int, SDL_Color, int);
    void draw_text_cached(std::string, std::string, int, int, SDL_Color, int);
    texture* add_to_atlas(SDL_Surface* arg);
    void draw_texture_(texture* Texture, int x, int y);
    GLuint new_texture_id();
	GLuint create_vbo(GLfloat* vertices, int size);
	void setup_vbo(){};
	void build_atlas();

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
    ~renderer_opengl() override;

};

#endif
