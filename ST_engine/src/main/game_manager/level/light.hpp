/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#ifndef LIGHT_DEF
#define LIGHT_DEF

#include <SDL2/SDL.h>

namespace ST {
    ///Contains the data needed to represent a light source.
    class light {
    public:
        int origin_x;
        int origin_y;
        uint16_t radius;
        uint16_t intensity;
        uint16_t brightness;

        light(int origin_x, int origin_y, uint16_t radius, uint16_t intensity, uint16_t brightness);
    };
}

#endif
