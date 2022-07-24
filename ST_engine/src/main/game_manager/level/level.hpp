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

#define PARALLAX_BG_LAYERS 4

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

        static key key_index(const std::string& arg);

    public:

        //public data
        /*
         * Contains the mappings from keys to actions.
         * Contains all entities, lights and text objects.
         * Contains the background overlay and the camera.
         */
        ska::bytell_hash_map<uint16_t , std::vector<ST::key>> actions_buttons{};
        std::vector<ST::entity> entities{};
        std::vector<ST::light> lights{};
        std::vector<ST::text> text_objects{};
        uint16_t background [PARALLAX_BG_LAYERS] = {65535, 65535, 65535, 65535};
        uint8_t parallax_speed [PARALLAX_BG_LAYERS] = {0, 0, 0, 0};
        SDL_Color background_color{0, 0, 0, 0};
        uint16_t overlay = 65535;
        uint8_t overlay_sprite_num = 1;
        uint32_t physics_objects_count = 0;

       // ST::camera camera = {0, 0, -1, 1920, 0, 1080};
        ST::camera camera = {0, 0, -1, 1920, 0, 1080};
        level(const std::string&, message_bus*);
        int8_t load();
        void reload();
        void unload();
        [[nodiscard]] std::string get_name() const;
        ~level();

        int8_t load_input_conf();
    };
}

#endif
