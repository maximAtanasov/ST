/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
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

class level_tests;

namespace ST {
    ///This object contains all the data for a level and provides functions for loading and unloading a level.
    /**
     * Sends messages to load assets and loads the input configuration from disk.
     */
    class level {
        friend class ::level_tests;
    private:

        std::string name;
        ST::level_data data;
        message_bus *gMessage_bus;

        int8_t load_input_conf();
        key key_index(std::string arg);

    public:
        level(const std::string&, message_bus*);
        void load();
        void unload();
        level_data *get_data();
        std::string get_name() const;
        ~level();
    };
}

//INLINED METHODS

/**
 * Returns the data of the level (entities, lights, background, text objects).
 * @return a <b>ST::level_data</b> struct.
 */
inline ST::level_data *ST::level::get_data() {
    return &data;
}

#endif
