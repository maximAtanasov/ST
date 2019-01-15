/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#ifndef ENTITY_DEF
#define ENTITY_DEF


#include <cstdint>
#include <cstddef>

namespace ST {

    ///This class represents all static or dynamic objects in the game (excluding text, see ST::text)
    /**
     * Contains all the data for an entity.
     * Texture and animation information, collisions box and position as well as various other options.
     */
    class entity {

    private:

        // 4 bytes
        int16_t col_x = 0;
        int16_t col_y = 0;

        //4 bytes
        int16_t offset_x = 0;
        int16_t offset_y = 0;

    public:

        //8 bytes
        int32_t x = 0;
        int32_t y = 0;

        //8 bytes
        size_t texture = 0;

        //8 bytes
        float tex_scale_x = 1;
        float tex_scale_y = 1;

        //4 bytes
        uint16_t tex_w = 0;
        uint16_t tex_h = 0;

        //2 bytes
        uint8_t sprite_num = 0;
        uint8_t animation = 0;

        //2 bytes
        int8_t velocity_x = 0;
        int8_t velocity_y = 0;

        //2 bytes
        uint8_t mass = 0;
        uint8_t animation_num = 0;

        //2 byte padding
        uint16_t padding_bytes = 0;

        //4 bytes
        bool is_active = true;
        bool is_static = false;//does not move with Camera
        bool is_visible = true;
        bool is_affected_by_physics = false;

        entity() = default;

        int32_t get_col_x() const;
        int32_t get_col_y() const;
        int16_t get_col_y_offset() const;
        int16_t get_col_x_offset() const;
        bool collides(const entity&) const;
        void set_collision_box(int16_t, int16_t, int16_t, int16_t);
    };
}

//INLINED METHODS

/**
 * Get the horizontal length of the collision box.
 * @return A int32_t representing the length.
 */
inline int32_t ST::entity::get_col_x() const{
    return col_x;
}

/**
 * Get the vertical length of the collision box.
 * @return A int32_t representing the length.
 */
inline int32_t ST::entity::get_col_y() const{
    return col_y;
}

/**
 * Get the horizontal offset for the collision box.
 * @return The offset relative to the current position.
 */
inline int16_t ST::entity::get_col_x_offset() const{
    return offset_x;
}

/**
 * Get the vertical offset for the collision box.
 * @return The offset relative to the current position.
 */
inline int16_t ST::entity::get_col_y_offset() const{
    return offset_y;
}

/**
 * Set the collision box for the entity, relative to the current position.
 * @param offsetX The horizontal offset for the colllision box.
 * @param offsetY The vertical offset for the colllision box.
 * @param X The horizontal length of the collision box.
 * @param Y The vertical length of the collision box.
 */
inline void ST::entity::set_collision_box(int16_t offsetX, int16_t offsetY, int16_t col_x, int16_t col_y){
    this->col_x = col_x;
    this->col_y = -col_y;
    offset_x = offsetX;
    offset_y = offsetY;
}

/**
 * Tells if two entities are colliding or not.
 * @param other Entity to test collision against.
 * @return True if colliding, false otherwise.
 */
inline bool ST::entity::collides(const entity& other) const{
    return !((y + offset_y < other.y + other.col_y + other.offset_y) || (y + col_y + offset_y > other.y + other.offset_y)
          || (x + offset_x > other.x + other.col_x + other.offset_x) || (x + col_x + offset_x < other.x + other.offset_x));
}

#endif
