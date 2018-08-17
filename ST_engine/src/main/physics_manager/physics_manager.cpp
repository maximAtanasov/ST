/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <physics_manager/physics_manager.hpp>

/**
 * Initializes the physics manager.
 * @param msg_bus A pointer to the global message bus.
 * @param tsk_mngr A pointer to the global task_mngr.
 */
physics_manager::physics_manager(message_bus *msg_bus, task_manager *tsk_mngr){
    gMessage_bus = msg_bus;
    gTask_manager = tsk_mngr;
    gMessage_bus->subscribe(SET_GRAVITY, &msg_sub);
    gMessage_bus->subscribe(SET_FRICTION, &msg_sub);
	gMessage_bus->subscribe(SET_FLOOR, &msg_sub);
	gMessage_bus->subscribe(PAUSE_PHYSICS, &msg_sub);
	gMessage_bus->subscribe(UNPAUSE_PHYSICS, &msg_sub);
    gravity = 0;
    friction = 3;
    level_floor = 0;
}

/**
 * Process horizontal collisions for all entities.
 */
void physics_manager::process_horizontal() {
    for(auto& i : *entities) {
        //handle horizontal velocity
        if (i.is_affected_by_physics()) {
            if (i.get_velocity_x() > 0) {
                for (int j = 0; j < i.get_velocity_x(); j++) {
                    if (entity_set_x(i.get_x() + 1, i.get_ID(), entities) == 0) {
                        break;
                    }
                }
                for (int j = 0; j < friction && i.get_velocity_x() > 0; j++) {
                    (i.set_velocity_x(static_cast<int16_t>(i.get_velocity_x() - 1)));
                }
            } else if (i.get_velocity_x() < 0) {
                for (int j = 0; j > i.get_velocity_x(); j--) {
                    if (entity_set_x(i.get_x() - 1, i.get_ID(), entities) == 0) {
                        break;
                    }
                }
                for (int j = 0; j < friction && i.get_velocity_x() < 0; j++) {
                    (i.set_velocity_x(static_cast<int16_t>(i.get_velocity_x() + 1)));
                }
            }
        }
    }
}

/**
 * Process vertical collisions for all entities.
 */
void physics_manager::process_vertical() {
    for(auto& i : *entities) {
        if (i.is_affected_by_physics()) {
            //handle vertical velocity
            const int16_t objectVelocity = i.get_velocity_y() + gravity;
            if (objectVelocity < 0) {
                for (int j = 0; j > objectVelocity; j--) {
                    if (entity_set_y(i.get_y() - 1, i.get_ID(), entities) == 0) {
                        break;
                    }
                }
            } else if (objectVelocity > 0) {
                for (int j = 0; j < objectVelocity; j++) {
                    if (i.get_y() + i.get_col_y_offset() < level_floor) {
                        if (entity_set_y(i.get_y() + 1, i.get_ID(), entities) == 0) {
                            break;
                        }
                    }
                }
            }
            //decrease velocity of objects (apply gravity really)
            int16_t realVelocity = objectVelocity - gravity;
            if (realVelocity < 0) {
                i.set_velocity_y(static_cast<int16_t>(realVelocity + 2));
            }
        }
    }
}

/**
 * Retrieves messages from the subscriber object and
 * performs the appropriate actions.
 */
void physics_manager::handle_messages(){
    message* temp = msg_sub.get_next_message();
    while(temp != nullptr){
        if(temp->msg_name == SET_GRAVITY){
            gravity = *static_cast<int8_t*>(temp->get_data());
        }else if(temp->msg_name == SET_FLOOR){
            level_floor = *static_cast<int32_t*>(temp->get_data());
        }else if(temp->msg_name == SET_FRICTION){
            friction = *static_cast<int8_t*>(temp->get_data());
        }else if(temp->msg_name == PAUSE_PHYSICS){
            physics_paused = true;
        }else if(temp->msg_name == UNPAUSE_PHYSICS){
            physics_paused = false;
        }
        destroy_msg(temp);
        temp = msg_sub.get_next_message();
    }
}

/**
 * Sets the X position of an entity.
 * @param X The X position to set.
 * @param ID The ID of the entity.
 * @param entities All entities in the level.
 * @return 0 if there was no collision and X was set, 1 otherwise.
 */
int physics_manager::entity_set_x(int32_t X, uint64_t ID, std::vector<ST::entity>* entities){
    int currentX = entities->at(ID).get_x();
    entities->at(ID).set_x(X);
    if(entities->at(ID).is_affected_by_physics())
        if(check_collision(ID, entities) == 0){//if there is a collision, don't modify x
            entities->at(ID).set_x(currentX);
            return 0;
        }
    return 1;
}

/**
 * Sets the Y position of an entity.
 * @param Y The Y position to set.
 * @param ID The ID of the entity.
 * @param entities All entities in the level.
 * @return 0 if there was no collision and X was set, 1 otherwise.
 */
int physics_manager::entity_set_y(int32_t Y, uint64_t ID, std::vector<ST::entity>* entities){
    int currentY = entities->at(ID).get_y();
    entities->at(ID).set_y(Y);
    if(entities->at(ID).is_affected_by_physics()){
        if(check_collision(ID, entities) == 0){//if there is a collision, don't modify y
            entities->at(ID).set_y(currentY);
            return 0;
        }
	}
    return 1;
}

/**
 * Checks if an entity collides with any other entities.
 * @param ID The ID of the entity to check.
 * @param entities All entities in the current level.
 * @return 0 if there was a collision, -1 otherwise.
 */
int physics_manager::check_collision(uint64_t ID, std::vector<ST::entity>* entities){
    for(unsigned int i = 0; i < entities->size(); i++){
        ST::entity* temp = &entities->at(i);
        if(temp->is_affected_by_physics()){
            if(ID == i){
                continue;
            }
            if(temp->collides(entities->at(ID))){
                return 0;
            }
        }
    }
    return -1;
}
