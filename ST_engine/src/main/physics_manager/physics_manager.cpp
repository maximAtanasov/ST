/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */
#include <physics_manager/physics_manager.hpp>

static bool singleton_initialized = false;
/**
 * Initializes the physics manager.
 * @param msg_bus A pointer to the global message bus.
 * @param tsk_mngr A pointer to the global task_mngr.
 */
physics_manager::physics_manager(message_bus &gMessageBus) : gMessage_bus(gMessageBus) {
    if(singleton_initialized){
        throw std::runtime_error("The phsyics manager cannot be initialized more than once!");
    }else{
        singleton_initialized = true;
    }
    gMessage_bus.subscribe(SET_GRAVITY, &msg_sub);
    gMessage_bus.subscribe(SET_FRICTION, &msg_sub);
	gMessage_bus.subscribe(SET_FLOOR, &msg_sub);
	gMessage_bus.subscribe(PAUSE_PHYSICS, &msg_sub);
	gMessage_bus.subscribe(UNPAUSE_PHYSICS, &msg_sub);
    gravity = 0;
    friction = 4;
    level_floor = 0;
}

/**
 * Process horizontal collisions for all entities.
 */
void physics_manager::process_horizontal(std::vector<ST::entity>* entities, int8_t friction) {
    for(uint64_t k = 0; k < entities->size(); k++) {
        auto& entity = entities->operator[](k);
        //handle horizontal velocity
        if (entity.is_affected_by_physics()) {
            if (entity.velocity_x > 0) {
                for (int j = 0; j < entity.velocity_x; j++) {
                    if (entity_set_x(entity.x + 1, k, entities) == 0) {
                        entity.velocity_x = 0;
                        break;
                    }
                }
                for (int j = 0; j < friction && entity.velocity_x > 0; j++) {
                    (entity.velocity_x = (static_cast<int8_t>(entity.velocity_x - 1)));
                }
            } else if (entity.velocity_x < 0) {
                for (int j = 0; j > entity.velocity_x; j--) {
                    if (entity_set_x(entity.x - 1, k, entities) == 0) {
                        entity.velocity_x = 0;
                        break;
                    }
                }
                for (int j = 0; j < friction && entity.velocity_x < 0; j++) {
                    (entity.velocity_x = (static_cast<int8_t>(entity.velocity_x + 1)));
                }
            }
        }
    }
}

physics_manager::~physics_manager() {
    singleton_initialized = false;
}

/**
 * Process vertical collisions for all entities.
 */
void physics_manager::process_vertical(std::vector<ST::entity>* entities, int8_t gravity, int32_t level_floor) {
    for(uint64_t k = 0; k < entities->size(); k++) {
        auto& entity = entities->operator[](k);
        if (entity.is_affected_by_physics()) {
            //handle vertical velocity
            const int8_t objectVelocity = entity.velocity_y + gravity;
            if (objectVelocity < 0) {
                for (int j = 0; j > objectVelocity; j--) {
                    if (entity_set_y(entity.y - 1, k, entities) == 0) {
                        break;
                    }
                }
            } else if (objectVelocity > 0) {
                for (int j = 0; j < objectVelocity; j++) {
                    if (entity.y + entity.get_col_y_offset() < level_floor) {
                        if (entity_set_y(entity.y + 1, k, entities) == 0) {
                            break;
                        }
                    }
                }
            }
            //decrease velocity of objects (apply gravity really)
            int8_t realVelocity = objectVelocity - gravity;
            if (realVelocity < 0) {
                entity.velocity_y = (static_cast<int8_t>(realVelocity + 2));
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
        switch (temp->msg_name) {
            case SET_GRAVITY:
                gravity = static_cast<int8_t>(temp->base_data0);
                break;
            case SET_FLOOR:
                level_floor = static_cast<int32_t>(temp->base_data0);
                break;
            case SET_FRICTION:
                friction = static_cast<int8_t>(temp->base_data0);
                break;
            case PAUSE_PHYSICS:
                physics_paused = true;
                break;
            case UNPAUSE_PHYSICS:
                physics_paused = false;
                break;
        }
        delete temp;
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
    ST::entity* entity = &entities->operator[](ID);
    int32_t currentX = entity->x;
    entity->x = X;
    if(check_collision(ID, entities) == 0){//if there is a collision, don't modify x
        entity->x = currentX;
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
    ST::entity* entity = &entities->operator[](ID);
    int32_t currentY = entity->y;
    entity->y = Y;
    if(check_collision(ID, entities) == 0){//if there is a collision, don't modify y
        entity->y = currentY;
        return 0;
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
        ST::entity* temp = &entities->operator[](i);
        if(temp->is_affected_by_physics()){
            if(i != ID && temp->collides(entities->operator[](ID))){
                return 0;
            }
        }
    }
    return -1;
}