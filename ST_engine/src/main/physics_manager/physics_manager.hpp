#include <utility>

/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#ifndef PHYSICS_DEF
#define PHYSICS_DEF

#include <game_manager/level/entity.hpp>
#include <message_bus.hpp>
#include <task_manager.hpp>

///This class handles all physics related actions in the engine.
class physics_manager{
    private:
        message_bus* gMessage_bus{};
        std::vector<ST::entity>* entities{};
		task_manager* gTask_manager{};
        subscriber msg_sub{};
        int32_t level_floor = 0;
		bool physics_paused = false;
        int8_t gravity = 0;
        int8_t friction = 0;


        static int check_collision(uint64_t, std::vector<ST::entity>* entities);
        static int entity_set_x(int32_t x, uint64_t, std::vector<ST::entity>* entities);
        static int entity_set_y(int32_t y, uint64_t, std::vector<ST::entity>* entities);

		void process_horizontal(std::vector<ST::entity>* entities);
		void process_vertical(std::vector<ST::entity>* entities);
        void handle_messages();


    public:
        physics_manager(message_bus* msg_bus, task_manager* tsk_mngr);
        void update(std::vector<ST::entity>* data);
        ~physics_manager();
};

//INLINED METHODS

/**
 * Responds to messages from the subscriber object and updates the physics if they are not paused.
 * @param data A pointer to the level data. (containing the entities that we need).
 */
inline void physics_manager::update(std::vector<ST::entity>* data){
    handle_messages();
    if(!physics_paused){
        process_horizontal(data);
        process_vertical(data);
    }
    auto end = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
}

#endif //PHYSICS_DEF
