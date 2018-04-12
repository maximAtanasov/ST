#include <game_manager/level/light.hpp>

ST::light::light(int origin_x, int origin_y, Uint16 radius, Uint16 intensity, Uint16 brightness){
    this->origin_x = origin_x;
    this->origin_y = origin_y;
    this->radius = radius;
    this->intensity = intensity;
    this->brightness = brightness;
}