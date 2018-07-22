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
    class light {
    private:
        uint64_t ID;
    public:
        int32_t origin_x;
        int32_t origin_y;
        uint16_t radius;
        uint16_t intensity;
        uint16_t brightness;

        light(uint64_t ID, int32_t origin_x, int32_t origin_y, uint16_t radius, uint16_t intensity, uint16_t brightness);
        uint64_t get_ID();

        int32_t get_origin_x() const;
        void set_origin_x(int32_t origin_x);

        int32_t get_origin_y() const;
        void set_origin_y(int32_t origin_y);

        uint16_t get_radius() const;
        void set_radius(uint16_t radius);

        uint16_t get_intensity() const;
        void set_intensity(uint16_t intensity);

        uint16_t get_brightness() const;
        void set_brightness(uint16_t brightness);

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

/**
 *
 * @return The x origin of the light.
 */
inline int32_t ST::light::get_origin_x() const {
    return origin_x;
}

/**
 * Sets the X origin of a light.
 * @param origin_x The value of the X coordinate.
 */
inline void ST::light::set_origin_x(int32_t origin_x) {
    light::origin_x = origin_x;
}

/**
 *
 * @return The y origin of the light.
 */
inline int32_t ST::light::get_origin_y() const {
    return origin_y;
}

/**
 * Sets the Y origin of a light.
 * @param origin_y The value of the Y coordinate.
 */
inline void ST::light::set_origin_y(int32_t origin_y) {
    light::origin_y = origin_y;
}

/**
 *
 * @return The radius of the light.
 */
inline uint16_t ST::light::get_radius() const {
    return radius;
}

/**
 * Sets the radius of the light.
 * @param radius The value of the radius.
 */
inline void ST::light::set_radius(uint16_t radius) {
    light::radius = radius;
}

/**
 *
 * @return The intensity of a light.
 */
inline uint16_t ST::light::get_intensity() const {
    return intensity;
}

/**
 * Sets the intensity of a light.
 * @param intensity The value of the intensity.
 */
inline void ST::light::set_intensity(uint16_t intensity) {
    light::intensity = intensity;
}

/**
 *
 * @return The brightness of the light.
 */
inline uint16_t ST::light::get_brightness() const {
    return brightness;
}

/**
 * Sets the brightness of a light.
 * @param brightness The value of the brightness.
 */
inline void ST::light::set_brightness(uint16_t brightness) {
    light::brightness = brightness;
}


#endif //LIGHT_DEF
