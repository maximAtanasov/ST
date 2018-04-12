#ifndef ENTITY_DEF
#define ENTITY_DEF

#include <defs.hpp>

namespace ST {

    class entity {

        struct collision_data {
            int left;
            int right;
            int top;
            int bottom;
        };

    private:
        //general

        //64 bits
        int x = 0;
        int y = 0;

        //64 bits
        Uint64 ID;
        bool visibility = true;
        bool static_ = false; //does not move with Camera
        int velocity_x = 0;
        int velocity_y = 0;

        //animation - 32 bits
        int sprite_num = 0;
        int animation = 0;
        int animation_num = 0;

        //texture
        size_t texture = 0;
        int tex_w = 0;
        int tex_h = 0;


        //physics
        int col_x = 0;
        int col_y = 0;
        int mass = 0;
        int offset_x = 0;
        int offset_y = 0;
        collision_data collision;
        bool affected_by_physics = false;
        bool active = true;

    public:
        //general
        explicit entity(unsigned int);

        void set_active(bool);

        bool is_active();

        bool is_static();

        void set_x(int);

        void set_y(int);

        int get_x();

        int get_y();

        Uint64 get_ID();

        void set_static(bool);

        void set_velocity_x(int);

        void set_velocity_y(int);

        int get_velocity_x();

        int get_velocity_y();

        //texture
        int get_tex_w();

        int get_tex_h();

        void set_tex_w(int);

        void set_tex_h(int);

        void set_visible(bool);

        bool is_visible();

        size_t get_texture();

        void set_texture(std::string);

        //physics
        int get_col_x();

        int get_col_y();

        int get_col_y_offset();

        int get_col_x_offset();

        bool collides(entity);

        void set_collision_box(int, int, int, int);

        void update_collision_box(int, int);

        void set_affected_by_physics(bool);

        bool is_affected_by_physics();

        int get_mass();

        void set_mass(int);

        //animation
        void set_animation_num(int);

        void set_sprite_num(int);

        void set_animation(int);

        int get_sprite_num();

        int get_animation();

        int get_animation_num();


    };
}

//INLINED METHODS

//GENERAL FUNCTIONS//
inline Uint64 ST::entity::get_ID(){
    return ID;
}

inline void ST::entity::set_active(bool arg){
    active = arg;
}

inline bool ST::entity::is_active(){
    return active;
}

inline int ST::entity::get_x(){
    return x;
}

inline int ST::entity::get_y(){
    return y;
}

inline void ST::entity::set_x(int X){
    x = X;
    update_collision_box(x, y);
}

inline void ST::entity::set_y(int Y){
    y = Y;
    update_collision_box(x, y);
}

inline void ST::entity::set_static(bool arg){
    static_ = arg;
}

inline bool ST::entity::is_static(){
    return static_;
}

inline void ST::entity::set_velocity_x(int arg){
    velocity_x = arg;
}

inline void ST::entity::set_velocity_y(int arg){
    velocity_y = arg;
}

inline int ST::entity::get_velocity_x(){
    return velocity_x;
}

inline int ST::entity::get_velocity_y(){
    return velocity_y;
}

//TEXTURE FUNCTIONS//

inline int ST::entity::get_tex_w(){
    return tex_w;
}

inline int ST::entity::get_tex_h(){
    return tex_h;
}

inline void ST::entity::set_tex_w(int width){
    tex_w = width;
}

inline void ST::entity::set_tex_h(int height){
    tex_h = height;
}

inline size_t ST::entity::get_texture(){
    return texture;
}

inline void ST::entity::set_visible(bool arg){
    visibility = arg;
}

inline void ST::entity::set_texture(std::string arg){
    std::hash<std::string> hash_f;
    texture = hash_f(arg);
}

inline bool ST::entity::is_visible(){
    return visibility;
}

//PHYSICS FUNTIONS//

inline int ST::entity::get_col_x(){
    return col_x;
}

inline int ST::entity::get_col_y(){
    return col_y;
}

inline int ST::entity::get_col_x_offset(){
    return offset_x;
}

inline int ST::entity::get_col_y_offset(){
    return offset_y;
}

inline void ST::entity::set_collision_box(int offsetX, int offsetY, int X, int Y){
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
    update_collision_box(x, y);
}

inline void ST::entity::update_collision_box(int x, int y){
    collision.left = x + offset_x;
    collision.right = x + col_x + offset_x;
    collision.top = y + col_y + offset_y;
    collision.bottom = y + offset_y;
}

inline void ST::entity::set_affected_by_physics(bool arg){
    affected_by_physics = arg;
}

inline bool ST::entity::is_affected_by_physics(){
    return affected_by_physics;
}

inline bool ST::entity::collides(entity other){
    return !((collision.bottom < other.collision.top) || (collision.top > other.collision.bottom)
          || (collision.left > other.collision.right) || (collision.right < other.collision.left));
}

inline void ST::entity::set_mass(int arg){
    mass = arg;
}

inline int ST::entity::get_mass(){
    return mass;
}

//ANIMATION FUNCTIONS//

inline void ST::entity::set_animation(int arg){
    animation = arg;
}

inline int ST::entity::get_animation(){
    return animation;
}

inline void ST::entity::set_animation_num(int arg){
    animation_num = arg;
}

inline int ST::entity::get_animation_num(){
    return animation_num;
}

inline void ST::entity::set_sprite_num(int arg){
    sprite_num = arg;
}

inline int ST::entity::get_sprite_num(){
    return sprite_num;
}

#endif
