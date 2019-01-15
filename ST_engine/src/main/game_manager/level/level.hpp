/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#ifndef LVL_DEF
#define LVL_DEF

#include <key_definitions.hpp>
#include <game_manager/level/entity.hpp>
#include <game_manager/level/text.hpp>
#include <game_manager/level/light.hpp>
#include <message_bus.hpp>
#include <game_manager/level/camera.hpp>

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
        message_bus *gMessage_bus;

        key key_index(std::string arg);

    public:

        //public data
        /*
         * Contains the mappings from keys to actions.
         * Contains all entities, lights and text objects.
         * Contains the background overlay and the camera.
         */
        ska::bytell_hash_map<size_t, std::vector<ST::key>> actions_Buttons{};
        std::vector<ST::entity> entities{};
        std::vector<ST::light> lights{};
        std::vector<ST::text> text_objects{};
        size_t background = 0;
        size_t overlay = 0;
        int16_t overlay_spriteNum = 1;
        ST::camera Camera = {0, 0, -1, 1920, 0, 1080};

        level(const std::string&, message_bus*);
        void load();
        void reload();
        void unload();
        std::string get_name() const;
        ~level();
        int8_t load_input_conf();
    };
}

#endif
