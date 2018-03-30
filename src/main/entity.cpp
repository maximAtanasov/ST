#include "entity.hpp"

//default constructor
entity::entity(unsigned int id){
    ID = id;
}

//GENERAL FUNCTIONS//
Uint64 entity::get_ID(){
    return ID;
}

void entity::set_active(bool arg){
	active = arg;
}

bool entity::is_active(){
	return active;
}

int entity::get_x(){
    return x;
}

int entity::get_y(){
    return y;
}

void entity::set_x(int X){
    x = X;
    update_collision_box(x, y);
}

void entity::set_y(int Y){
    y = Y;
    update_collision_box(x, y);
}

void entity::set_static(bool arg){
    static_ = arg;
}

bool entity::is_static(){
    return static_;
}

void entity::set_velocity_x(int arg){
    velocity_x = arg;
}

void entity::set_velocity_y(int arg){
    velocity_y = arg;
}

int entity::get_velocity_x(){
    return velocity_x;
}

int entity::get_velocity_y(){
    return velocity_y;
}

//TEXTURE FUNCTIONS//

int entity::get_tex_w(){
    return tex_w;
}

int entity::get_tex_h(){
    return tex_h;
}

void entity::set_tex_w(int width){
    tex_w = width;
}

void entity::set_tex_h(int height){
    tex_h = height;
}

size_t entity::get_texture(){
    return texture;
}

void entity::set_visible(bool arg){
    visibility = arg;
}

void entity::set_texture(std::string arg){
    std::hash<std::string> hash_f;
    texture = hash_f(arg);
}

bool entity::is_visible(){
    return visibility;
}

//PHYSICS FUNTIONS//

int entity::get_col_x(){
    return col_x;
}

int entity::get_col_y(){
    return col_y;
}

int entity::get_col_x_offset(){
	return offset_x;
}

int entity::get_col_y_offset(){
	return offset_y;
}

void entity::set_collision_box(int offsetX, int offsetY, int X, int Y){
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

void entity::update_collision_box(int x, int y){
    collision.left = x + offset_x;
    collision.right = x + col_x + offset_x;
    collision.top = y + col_y + offset_y;
    collision.bottom = y + offset_y;
}

void entity::set_affected_by_physics(bool arg){
    affected_by_physics = arg;
}

bool entity::is_affected_by_physics(){
    return affected_by_physics;
}

bool entity::collides(entity other){
    return !((collision.bottom < other.collision.top) | (collision.top > other.collision.bottom)
    | (collision.left > other.collision.right) | (collision.right < other.collision.left));
}

void entity::set_mass(int arg){
    mass = arg;
}

int entity::get_mass(){
    return mass;
}

//ANIMATION FUNCTIONS//

void entity::set_animation(int arg){
    animation = arg;
}

int entity::get_animation(){
    return animation;
}

void entity::set_animation_num(int arg){
    animation_num = arg;
}

int entity::get_animation_num(){
    return animation_num;
}

void entity::set_sprite_num(int arg){
    sprite_num = arg;
}

int entity::get_sprite_num(){
    return sprite_num;
}