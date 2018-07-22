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
            int32_t left;
            int32_t right;
            int32_t top;
            int32_t bottom;
        };

    private:
        //general

        int32_t x = 0;
        int32_t y = 0;

        uint64_t ID;
        bool visibility = true;
        bool static_ = false; //does not move with Camera
        int16_t velocity_x = 0;
        int16_t velocity_y = 0;

        //animation
        uint16_t sprite_num = 0;
        uint16_t animation = 0;
        uint16_t animation_num = 0;

        //texture
        size_t texture = 0;
        uint16_t tex_w = 0;
        uint16_t tex_h = 0;

        //physics
        int32_t col_x = 0;
        int32_t col_y = 0;
        uint16_t mass = 0;
        int16_t offset_x = 0;
        int16_t offset_y = 0;
        collision_box collision;
        bool affected_by_physics = false;
        bool active = true;

        void update_collision_box();

    public:
        //general
        explicit entity(unsigned int);
        void set_active(bool);
        bool is_active() const;
        bool is_static() const;
        void set_x(int32_t);
        void set_y(int32_t);
        int32_t get_x() const;
        int32_t get_y() const;
        uint64_t get_ID() const;
        void set_static(bool);
        void set_velocity_x(int16_t);
        void set_velocity_y(int16_t);
        int16_t get_velocity_x() const;
        int16_t get_velocity_y() const;

        //texture
        uint16_t get_tex_w() const;
        uint16_t get_tex_h() const;
        void set_tex_w(uint16_t);
        void set_tex_h(uint16_t);
        void set_visible(bool);
        bool is_visible() const;
        size_t get_texture() const;
        void set_texture(const std::string&);

        //physics
        int32_t get_col_x() const;
        int32_t get_col_y() const;
        int16_t get_col_y_offset() const;
        int16_t get_col_x_offset() const;
        bool collides(entity) const;
        void set_collision_box(int16_t, int16_t, int16_t, int16_t);

        void set_affected_by_physics(bool);
        bool is_affected_by_physics() const;
        int get_mass() const;

        void set_mass(uint16_t);
        //animation
        void set_animation_num(uint16_t);
        void set_sprite_num(uint16_t);
        void set_animation(uint16_t);
        uint16_t get_sprite_num() const;
        uint16_t get_animation() const;
        uint16_t get_animation_num() const;
    };
}





//INLINED METHODS

//GENERAL FUNCTIONS//

/**
 *
 * @return The ID of the entity.
 */
inline uint64_t ST::entity::get_ID() const{
    return ID;
}

/**
 * Sets the entity to active state or not.
 * @param arg True or false.
 */
inline void ST::entity::set_active(bool arg){
    active = arg;
}

/**
 * Returns the status of the entity.
 * @return Boolean indicating if the entity is active or not.
 */
inline bool ST::entity::is_active() const{
    return active;
}

/**
 * Returns the x coordinate of the entity.
 * @return A signed 32-bit integer representing the current X coordinate position of the entity.
 */
inline int32_t ST::entity::get_x() const{
    return x;
}

/**
 * Returns the y coordinate of the entity. (y = 0 is the top of the screen)
 * @return A signed 32-bit integer representing the current Y coordinate position of the entity.
 */
inline int32_t ST::entity::get_y() const{
    return y;
}

/**
 *
 * @param X Set the x coordinate of the entity.
 */
inline void ST::entity::set_x(int32_t X){
    x = X;
    update_collision_box();
}

/**
 *
 * @param X Set the y coordinate of the entity.
 */
inline void ST::entity::set_y(int32_t Y){
    y = Y;
    update_collision_box();
}

/**
 * Sets the entity to static (not moving with the camera) or not static (following the camera).
 * @param arg True or False.
 */
inline void ST::entity::set_static(bool arg){
    static_ = arg;
}

/**
 *
 * @return A boolean indicating if the entity is static or not.
 */
inline bool ST::entity::is_static() const{
    return static_;
}

/**
 * Sets the current x velocity of the entity.
 * @param arg A signed 16-bit integer representing the velocity (positive or negative)
 */
inline void ST::entity::set_velocity_x(int16_t arg){
    velocity_x = arg;
}

/**
 * Sets the current y velocity of the entity.
 * @param arg A signed 16-bit integer representing the velocity (positive or negative)
 */
inline void ST::entity::set_velocity_y(int16_t arg){
    velocity_y = arg;
}

/**
 * Get the current x velocity of the entity.
 * @return A signed 16-bit integer representing the velocity (positive or negative)
 */
inline int16_t ST::entity::get_velocity_x() const{
    return velocity_x;
}

/**
 * Get the current y velocity of the entity.
 * @return A signed 16-bit integer representing the velocity (positive or negative)
 */
inline int16_t ST::entity::get_velocity_y() const{
    return velocity_y;
}

//TEXTURE FUNCTIONS//

/**
 * Get the width of the texture of the entity.
 * @return An unsigned 16-bit integer representing the texture width.
 */
inline uint16_t ST::entity::get_tex_w() const{
    return tex_w;
}

/**
 * Get the height of the texture of the entity.
 * @return An unsigned 16-bit integer representing the texture height.
 */
inline uint16_t ST::entity::get_tex_h() const{
    return tex_h;
}

/**
 * Sets the width of the texture of the entity.
 * @param width An unsigned 16-bit integer representing the texture width.
 */
inline void ST::entity::set_tex_w(uint16_t width){
    tex_w = width;
}

/**
 * Sets the height of the texture of the entity.
 * @param height An unsigned 16-bit integer representing the texture height.
 */
inline void ST::entity::set_tex_h(uint16_t height){
    tex_h = height;
}

/**
 * Get the texture of the entity.
 * @return An unsigned integer representing the hash of the texture name.
 */
inline size_t ST::entity::get_texture() const{
    return texture;
}

/**
 * Sets the entity to visible or not visible.
 * @param arg True = visible, false = not visible.
 */
inline void ST::entity::set_visible(bool arg){
    visibility = arg;
}

/**
 * Sets the texture of the current entity, takes a hash of the std::string and does not keep a copy.
 * @param arg A <b>std::string</b> containing the name of the texture.
 */
inline void ST::entity::set_texture(const std::string& arg){
    std::hash<std::string> hash_f;
    texture = hash_f(arg);
}

/**
 * Tells if the entity is visble or not.
 * @return True if visible, false otherwise.
 */
inline bool ST::entity::is_visible() const{
    return visibility;
}

//PHYSICS FUNTIONS//

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
        set_affected_by_physics(false);
    }
    update_collision_box();
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
 * Set whether or not the entity is affected by physics.
 * @param arg True = affected, false = not affected (skipped by the physics manager)
 */
inline void ST::entity::set_affected_by_physics(bool arg){
    affected_by_physics = arg;
}

/**
 * Tells if the entity is affected by physics or not.
 * @return True = affected, false = not affected (skipped by the physics manager)
 */
inline bool ST::entity::is_affected_by_physics() const{
    return affected_by_physics;
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

/**
 * Sets the mass of the entity.
 * @param arg The value of the mass.
 */
inline void ST::entity::set_mass(uint16_t arg){
    mass = arg;
}

/**
 * Get the mass of the entity.
 * @return The value of the mass.
 */
inline int ST::entity::get_mass() const{
    return mass;
}

//ANIMATION FUNCTIONS//

/**
 * Set the animation to play.
 * @param arg The number of the animation. (from 1 to n, where n is the rows of your
 * sprite sheet and each row contains one animation).
 */
inline void ST::entity::set_animation(uint16_t arg){
    animation = arg;
}

/**
 * Get the currently playing animation.
 * @return The number of the animation. (from 1 to n, where n is the rows of your
 * sprite sheet and each row contains one animation).
 */
inline uint16_t ST::entity::get_animation() const{
    return animation;
}

/**
 * Set the number of animations the entity has.
 * @param arg Equal to the number of rows of your spritesheet.
 */
inline void ST::entity::set_animation_num(uint16_t arg){
    animation_num = arg;
}

/**
 * Get the number of animations the entity has.
 * @return Equal to the number of rows of your spritesheet.
 */
inline uint16_t ST::entity::get_animation_num() const{
    return animation_num;
}

/**
 * Set the number of sprites the entity has. This is equal to the amount of colums your spritesheet has.
 * @param arg An integer from 1 to n, where n should be the amount of columns in your spritesheet.
 */
inline void ST::entity::set_sprite_num(uint16_t arg){
    sprite_num = arg;
}

/**
 * Get the number of sprites the entity has. This is equal to the amount of colums your spritesheet has.
 * @return An integer from 1 to n, where n should be the amount of columns in your spritesheet.
 */
inline uint16_t ST::entity::get_sprite_num() const{
    return sprite_num;
}

#endif
