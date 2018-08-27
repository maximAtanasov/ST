/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#ifndef LIGHT_DEF
#define LIGHT_DEF

#include <cstdint>

namespace ST {
    ///Contains the data needed to represent a light source.
    struct light {

    private:
        uint64_t ID;
    public:
        int32_t origin_x;
        int32_t origin_y;
        uint16_t radius;
        uint16_t intensity;
        uint16_t brightness;
        bool is_static = false;

        light(uint64_t ID, int32_t origin_x, int32_t origin_y, uint16_t radius, uint16_t intensity, uint16_t brightness);
        uint64_t get_ID();
    };
}

//INLINE METHODS

/**
 *
 * @return The ID of the light object
 */
inline uint64_t ST::light::get_ID(){
    return this->ID;
}
#endif //LIGHT_DEF
