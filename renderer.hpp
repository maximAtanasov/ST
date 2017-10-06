#ifndef DRAWING_DEF
#define DRAWING_DEF

#include "defs.hpp"
#include "font_cache.hpp"
#include "assets_manager.hpp"
#include "display_manager.hpp"

typedef struct Camera{
    int x;
    int y;
    int limitX1;
    int limitX2;
    int limitY1;
    int limitY2;
} Camera;


class renderer{
    SDL_Renderer* sdl_renderer;
    int vsync;
    font_cache gFont_cache = 100;
    void draw_text_normal(std::string, std::string, int, int, SDL_Color, int);
    void draw_text_cached(std::string, std::string, int, int, SDL_Color, int);
public:
    renderer(void);
    int initialize();
    void draw_texture(int, int, int);
    void draw_rectangle(int, int, int, int, SDL_Color);
    void draw_rectangle_filled(int, int, int, int, SDL_Color);
    void draw_background(int);
    void draw_sprite(int, int, int, int, int, int, int);
    void draw_text(std::string, std::string, int, int, SDL_Color, int, int*);
    int get_vsync(void);
    void vsync_on(void);
    void vsync_off(void);
    inline void present(void){
        SDL_RenderPresent(sdl_renderer);
    }
    void set_draw_color(int,int,int,int);
    inline void clear_screen(void){
        SDL_RenderClear(sdl_renderer);
    }
    SDL_Renderer* get_renderer(void);
    ~renderer();
    void close(void);
};

void drawEntities();
void draw(double extrapolation);
void drawCollisions();
void drawCoordinates(void);

extern bool collisionsShown;
extern renderer gRenderer;
extern Camera camera;
extern int* D_RENDERTEXT_SMOOTH;
extern int* D_RENDERTEXT_FAST;

#endif
