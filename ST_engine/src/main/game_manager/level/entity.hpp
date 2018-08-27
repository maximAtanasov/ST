/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#ifndef ENTITY_DEF
#define ENTITY_DEF


#include <cstdint>
#include <cstddef>
#include <string>

namespace ST {

    ///This class represents all static or dynamic objects in the game (excluding text, see ST::text)
    /**
     * Contains all the data for an entity.
     * Texture and animation information, collisions box and position as well as various other options.
     */
    class entity {

        ///a simple struct defining the collision box for an entity.
        struct collision_box {
            int32_t left{0};
            int32_t right{0};
            int32_t top{0};
            int32_t bottom{0};
        };

    private:
        uint64_t ID;
        int32_t col_x = 0;
        int32_t col_y = 0;
        int16_t offset_x = 0;
        int16_t offset_y = 0;
        collision_box collision;

    public:

        //general
        int32_t x = 0;
        int32_t y = 0;
        int16_t velocity_x = 0;
        int16_t velocity_y = 0;
        bool is_active = true;
        bool is_static = false;//does not move with Camera
        bool is_visible = true;

        //texture
        size_t texture = 0;
        uint16_t tex_w = 0;
        uint16_t tex_h = 0;

        //animation
        uint16_t sprite_num = 0;
        uint16_t animation = 0;
        uint16_t animation_num = 0;

        //physics
        bool is_affected_by_physics = false;
        uint16_t mass = 0;

        explicit entity(unsigned int);
        uint64_t get_ID() const;

        //physics
        int32_t get_col_x() const;
        int32_t get_col_y() const;
        int16_t get_col_y_offset() const;
        int16_t get_col_x_offset() const;
        bool collides(entity) const;
        void set_collision_box(int16_t, int16_t, int16_t, int16_t);
        collision_box get_collision_box();
        void update_collision_box();
    };
}


//INLINED METHODS

/**
 *
 * @return The ID of the entity.
 */
inline uint64_t ST::entity::get_ID() const{
    return ID;
}



//PHYSICS METHODS//

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
inline void ST::entity::set_collision_box(int16_t offsetX, int16_t offsetY, int16_t X, int16_t Y){
    if(X != 0 && Y != 0){
        col_x = X - 1;
        col_y = -Y + 1;
        offset_x = offsetX;
        offset_y = offsetY;
    }else{
        col_x = 0;
        col_y = 0;
        is_affected_by_physics = false;
    }
    update_collision_box();
}

/**
 *
 * @return A copy of this entity's collision box.
 */
inline ST::entity::collision_box ST::entity::get_collision_box(){
    return this->collision;
}

/**
 * Updates the collision_box struct of the entity relative to it's current position.
 */
inline void ST::entity::update_collision_box(){
    collision.left = x + offset_x;
    collision.right = x + col_x + offset_x;
    collision.top = y + col_y + offset_y;
    collision.bottom = y + offset_y;
}

/**
 * Tells if two entities are colliding or not.
 * @param other Entity to test collision against.
 * @return True if colliding, false otherwise.
 */
inline bool ST::entity::collides(entity other) const{
    return !((collision.bottom < other.collision.top) || (collision.top > other.collision.bottom)
          || (collision.left > other.collision.right) || (collision.right < other.collision.left));
}

#endif
