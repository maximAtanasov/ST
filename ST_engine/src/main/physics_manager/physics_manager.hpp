/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

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
        std::vector<ST::entity>* entities{};


        int check_collision(unsigned int id, std::vector<ST::entity>* entities);
        int entity_set_x(int x, unsigned int ID, std::vector<ST::entity>* entities);
        int entity_set_y(int y, unsigned int ID, std::vector<ST::entity>* entities);

		void process_horizontal();
		void process_vertical();
        void handle_messages();

    public:
        physics_manager() = default;
        int initialize(message_bus* msg_bus, task_manager* tsk_mngr);
        void update(ST::level_data* data);
        void close();
};

//INLINED METHODS

inline void physics_manager::update(ST::level_data* data){
	handle_messages();
	if(physics_paused){
		return;
	}
	entities = &data->entities;
	process_horizontal();
	process_vertical();
}

#endif
