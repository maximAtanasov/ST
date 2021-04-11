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
    for(uint64_t k = 0; k < entities->size(); ++k) {
        auto& entity = entities->operator[](k);
        //handle horizontal velocity
        if (entity.is_affected_by_physics()) {
            if (entity.velocity_x > 0) {
                for (int j = 0; j < entity.velocity_x; ++j) {
                    //Branch-less check for whether x has been set.
                    entity.velocity_x = entity_set_x(entity.x + 1, k, entities) * entity.velocity_x; // NOLINT(cppcoreguidelines-narrowing-conversions)
                }
                for (int j = 0; j < friction && entity.velocity_x > 0; ++j) {
                    entity.velocity_x = static_cast<int8_t>(entity.velocity_x - 1);
                }
            } else if (entity.velocity_x < 0) {
                for (int j = 0; j > entity.velocity_x; --j) {
                    //Branch-less check for whether x has been set.
                    entity.velocity_x = entity_set_x(entity.x - 1, k, entities) * entity.velocity_x; // NOLINT(cppcoreguidelines-narrowing-conversions)
                }
                for (int j = 0; j < friction && entity.velocity_x < 0; ++j) {
                    entity.velocity_x = static_cast<int8_t>(entity.velocity_x + 1);
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
    for(uint64_t k = 0; k < entities->size(); ++k) {
        auto& entity = entities->operator[](k);
        if (entity.is_affected_by_physics()) {
            //handle vertical velocity
            const int8_t objectVelocity = entity.velocity_y + gravity;
            for (int j = 0; j > objectVelocity && entity_set_y(entity.y - 1, k, entities) != 0; --j);
            for (int j = 0; j < objectVelocity; ++j) {
                if (entity.y + entity.get_col_y_offset() < level_floor) {
                    if (entity_set_y(entity.y + 1, k, entities) == 0) {
                        break;
                    }
                }
            }
            //decrease velocity of objects (apply gravity really)
            int8_t realVelocity = objectVelocity - gravity;
            entity.velocity_y = (realVelocity < 0)*static_cast<int8_t>(realVelocity + 2) + (realVelocity >= 0)*entity.velocity_y;
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
uint8_t physics_manager::entity_set_x(int32_t X, uint64_t ID, std::vector<ST::entity>* entities){
    ST::entity* entity = &entities->operator[](ID);
    int32_t old_x = entity->x;
    entity->x = X;
    uint8_t collision = check_collision(ID, entities);
    entity->x = collision*old_x + !collision*entity->x; //if there is a collision, don't modify x
    return !collision;
}

/**
 * Sets the Y position of an entity.
 * @param Y The Y position to set.
 * @param ID The ID of the entity.
 * @param entities All entities in the level.
 * @return 0 if there was no collision and X was set, 1 otherwise.
 */
uint8_t physics_manager::entity_set_y(int32_t Y, uint64_t ID, std::vector<ST::entity>* entities){
    ST::entity* entity = &entities->operator[](ID);
    int32_t old_y = entity->y;
    entity->y = Y;
    uint8_t collision = check_collision(ID, entities);
    entity->y = collision*old_y + !collision*entity->y; //if there is a collision, don't modify y
    return !collision;
}

/**
 * Checks if an entity collides with any other entities.
 * @param ID The ID of the entity to check.
 * @param entities All entities in the current level.
 * @return 1 if there was a collision, 0 otherwise.
 */
int physics_manager::check_collision(uint64_t ID, std::vector<ST::entity>* entities){
    uint8_t result = 0;
    for(size_t i = 0; i < entities->size() && result == 0; i++){
        ST::entity* temp = &entities->operator[](i);
        result = temp->is_affected_by_physics() && i != ID && temp->collides(entities->operator[](ID));
    }
    return result;
}