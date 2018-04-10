#ifndef LIGHT_DEF
#define LIGHT_DEF

#include <SDL2/SDL.h>

class light{
    public:
        int origin_x;
        int origin_y;
        Uint16 radius;
        Uint16 intensity;
        Uint16 brightness;
        SDL_Color color{};
        light(int origin_x, int origin_y, Uint16 radius, Uint16 intensity, Uint16 brightness);
};

#endif
