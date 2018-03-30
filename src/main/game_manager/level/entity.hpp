#ifndef ENTITY_DEF
#define ENTITY_DEF

#include "../../defs.hpp"

typedef struct collision_data{
    int left;
    int right;
    int top;
    int bottom;
} collision_data;

class entity{
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

#endif
