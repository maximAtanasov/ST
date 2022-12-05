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
#include "game_manager/level/level.hpp"

///This class handles all physics related actions in the engine.
class physics_manager {
private:
    message_bus &gMessage_bus;
    subscriber msg_sub{};
    int32_t level_floor = 0;
    bool physics_paused = false;
    std::vector<ST::entity *> entities_ref{};
    int8_t gravity = 0;
    int8_t friction = 0;

    static int check_collision(uint64_t, std::vector<ST::entity *> *entities);

    static uint8_t entity_set_x(int32_t x, uint64_t, std::vector<ST::entity *> *entities);

    static uint8_t entity_set_y(int32_t y, uint64_t, std::vector<ST::entity *> *entities);

    static void process_horizontal(std::vector<ST::entity *> *entities, int8_t friction);

    static void process_vertical(std::vector<ST::entity *> *entities, int8_t gravity, int32_t level_floor);

    void handle_messages();

public:
    explicit physics_manager(message_bus &gMessageBus);

    void update(ST::level &level);

    ~physics_manager();

};

#endif //PHYSICS_DEF
