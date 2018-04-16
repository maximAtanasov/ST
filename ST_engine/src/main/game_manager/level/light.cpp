/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <game_manager/level/light.hpp>

ST::light::light(int origin_x, int origin_y, Uint16 radius, Uint16 intensity, Uint16 brightness){
    this->origin_x = origin_x;
    this->origin_y = origin_y;
    this->radius = radius;
    this->intensity = intensity;
    this->brightness = brightness;
}