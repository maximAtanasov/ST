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
     * Texture and animation information, position as well as various other options.
     */
    class entity {

    private:

        //4 bytes
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
        /*
        [0] is_active = true;
        [1] is_static = false; does not move with Camera
        [2] is_visible = true;
        [3] is_affected_by_physics;
         */
        uint8_t toggles = 0;
        uint8_t animation_num = 0;

        //2 bytes
        uint16_t texture = 65535;

        entity(){
            toggles |= (1U<<0U);
            toggles &= ~(1U<<1U);
            toggles |= (1U<<2U);
            toggles &= ~(1U<<3U);
        };

        [[nodiscard]] int32_t get_col_x() const;
        [[nodiscard]] int32_t get_col_y() const;
        [[nodiscard]] int16_t get_col_y_offset() const;
        [[nodiscard]] int16_t get_col_x_offset() const;
        [[nodiscard]] bool collides(const entity&) const;
        void set_collision_box(int16_t, int16_t, int16_t, int16_t);
        [[nodiscard]] bool is_active() const;
        [[nodiscard]] bool is_static() const;
        [[nodiscard]] bool is_visible() const;
        [[nodiscard]] bool is_affected_by_physics() const;
        void set_active(bool active);
        void set_static(bool static_);
        void set_visible(bool visible);
        void set_affected_by_physics(bool affected);
    };
}

//INLINED METHODS

inline bool ST::entity::is_active() const{
    return static_cast<bool>(toggles & (1U << 0U));
}

inline bool ST::entity::is_static() const{
    return static_cast<bool>(toggles & (1U << 1U));
}

inline bool ST::entity::is_visible() const{
    return static_cast<bool>(toggles & (1U << 2U));
}

inline bool ST::entity::is_affected_by_physics() const{
    return static_cast<bool>(toggles & (1U << 3U));
}

inline void ST::entity::set_active(bool active) {
    if(active){
        toggles |= (1U<<0U);
    }else{
        toggles &= ~(1U<<0U);
    }
}

inline void ST::entity::set_static(bool static_) {
    if(static_){
        toggles |= (1U<<1U);
    }else{
        toggles &= ~(1U<<1U);
    }
}

inline void ST::entity::set_visible(bool visible) {
    if(visible){
        toggles |= (1U<<2U);
    }else{
        toggles &= ~(1U<<2U);
    }
}

inline void ST::entity::set_affected_by_physics(bool affected) {
    if(affected){
        toggles |= (1U<<3U);
    }else{
        toggles &= ~(1U<<3U);
    }
}

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
 * @param offsetX The horizontal offset for the collision box.
 * @param offsetY The vertical offset for the collision box.
 * @param X The horizontal length of the collision box.
 * @param Y The vertical length of the collision box.
 */
inline void ST::entity::set_collision_box(int16_t offsetX, int16_t offsetY, int16_t col_x_, int16_t col_y_){
    this->col_x = col_x_;
    this->col_y = -col_y_;
    offset_x = offsetX;
    offset_y = offsetY;
}

/**
 * Tells if two entities are colliding or not.
 * @param other Entity to test collision against.
 * @return True if colliding, false otherwise.
 */
inline bool ST::entity::collides(const entity& other) const{
    return !((y + offset_y <= other.y + other.col_y + other.offset_y) || (y + col_y + offset_y >= other.y + other.offset_y)
          || (x + offset_x >= other.x + other.col_x + other.offset_x) || (x + col_x + offset_x <= other.x + other.offset_x));
}

#endif
