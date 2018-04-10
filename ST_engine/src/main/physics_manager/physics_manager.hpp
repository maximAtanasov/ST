#ifndef PHYSICS_DEF
#define PHYSICS_DEF

#include <defs.hpp>
#include <game_manager/level/level.hpp>
#include <game_manager/level/entity.hpp>
#include <task_manager/task_manager.hpp>

class physics_manager{
    private:
        message_bus* gMessage_bus{};
		task_manager* gTask_manager{};
        subscriber* msg_sub{};
        int gravity = 0;
        int friction = 0;
        int level_floor = 0;
		bool physics_paused = false;
        std::vector<entity>* entities{};


        int check_collision(unsigned int id, std::vector<entity>* entities);
        int entity_set_x(int x, unsigned int ID, std::vector<entity>* entities);
        int entity_set_y(int y, unsigned int ID, std::vector<entity>* entities);

		void process_horizontal();
		void process_vertical();
        void handle_messages();

    public:
        physics_manager() = default;
        int initialize(message_bus* msg_bus, task_manager* tsk_mngr);
        void update(level_data* data);
        void close();
};

//INLINED METHODS

inline void physics_manager::update(level_data* data){
	handle_messages();
	if(physics_paused){
		return;
	}
	entities = &data->entities;
	process_horizontal();
	process_vertical();
}

#endif
