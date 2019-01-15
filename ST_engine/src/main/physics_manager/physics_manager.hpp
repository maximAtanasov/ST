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
/**
 * Messages this subsystem listens to: <br>
 *
 * <b>SET_GRAVITY</b> - Sets the current gravity.
 *
 * Message must contain: a pointer to a <b>int8_t</b>.
 *
 *  <b>SET_FRICTION</b> - Sets the current friction.
 *
 * Message must contain: a pointer to a <b>int8_t</b>.
 *
 *  <b>SET_FLOOR</b> - Sets the floor of the level.
 *
 * Message must contain: a pointer to a <b>int32_t</b>.
 *
 *  <b>PAUSE_PHYSICS</b> - Pauses all physics.
 *
 * Message must contain: a <b>nullptr</b>.
 *
 * <b>UNPAUSE_PHYSICS</b> - Resumes all physics.
 *
 * Message must contain: a <b>nullptr</b>.
 */
class physics_manager{
    private:
        message_bus* gMessage_bus{};
		task_manager* gTask_manager{};
        subscriber msg_sub{};
        int8_t gravity = 0;
        int8_t friction = 0;
        int32_t level_floor = 0;
		bool physics_paused = false;
		std::vector<ST::entity>* entities;

        int check_collision(uint64_t, std::vector<ST::entity>* entities);
        int entity_set_x(int32_t x, uint64_t, std::vector<ST::entity>* entities);
        int entity_set_y(int32_t y, uint64_t, std::vector<ST::entity>* entities);

		void process_horizontal(std::vector<ST::entity>* entities);
		void process_vertical(std::vector<ST::entity>* entities);
        void handle_messages();


    public:
        physics_manager(message_bus* msg_bus, task_manager* tsk_mngr);
        void update(std::vector<ST::entity>* data);
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
