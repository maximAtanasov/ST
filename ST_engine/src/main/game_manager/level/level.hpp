/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#ifndef LVL_DEF
#define LVL_DEF

#include <input_manager/key_definitions.hpp>
#include <defs.hpp>
#include <game_manager/level/entity.hpp>
#include <game_manager/level/text.hpp>
#include <game_manager/level/light.hpp>
#include <message_bus/message_bus.hpp>
#include <game_manager/level/camera.hpp>
#include <input_manager/input_manager.hpp>
#include <game_manager/level/level_data.hpp>

namespace ST {
    class level {
    private:
        std::string name;
        ST::level_data data;
        message_bus *gMessage_bus;

        int load_input_conf();

        key key_index(std::string arg);

    public:
        level(const std::string &, message_bus *);

        void load();

        void unload();

        inline level_data *get_data() {
            return &data;
        }

        std::string get_name();

        ~level();
    };
}

#endif
