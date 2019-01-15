/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#include <game_manager/level/light.hpp>

/**
 * The constructor for a light object.
 * @param origin_x The X position of the light.
 * @param origin_y The Y position of the light.
 * @param radius The radius of the light.
 * @param intensity The intensity of the light.
 * @param brightness The brightness of the light.
 */
ST::light::light(int32_t origin_x, int32_t origin_y, uint16_t radius, uint16_t intensity, uint16_t brightness){
    this->origin_x = origin_x;
    this->origin_y = origin_y;
    this->radius = radius;
    this->intensity = intensity;
    this->brightness = brightness;
}