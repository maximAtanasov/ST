/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#ifndef LIGHT_DEF
#define LIGHT_DEF

#include <cstdint>

namespace ST {
    ///Contains the data needed to represent a light source.
    struct light {

    public:
        //8 bytes
        int32_t origin_x;
        int32_t origin_y;

        //4 bytes
        uint16_t radius;
        uint16_t intensity;

        //2 bytes
        uint16_t brightness;

        //2 bytes
        bool is_static = false;
        uint8_t padding_byte;

        light(int32_t origin_x, int32_t origin_y, uint16_t radius, uint16_t intensity, uint16_t brightness);
    };
    static_assert(sizeof(ST::light) == 16, "class 'text' is not sized properly, maybe you have misaligned the fields");
}

#endif //LIGHT_DEF
