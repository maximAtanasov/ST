/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#ifndef SLAVIC_TALES_LEVEL_DATA_HPP
#define SLAVIC_TALES_LEVEL_DATA_HPP

#include <input_manager/key_definitions.hpp>
#include <game_manager/level/entity.hpp>
#include <game_manager/level/light.hpp>
#include <game_manager/level/text.hpp>
#include <game_manager/level/camera.hpp>

namespace ST {
    ///Holds all the data for a level.
    /**
     * Contains the mappings from keys to actions.
     * Contains all entities, lights and text objects.
     * Contains the background overlay and the camera.
     */
    struct level_data {
        ska::bytell_hash_map<size_t, key> actions_Buttons{};
        std::vector<entity> entities{};
        std::vector<ST::light> lights{};
        std::vector<ST::text> text_objects{};
        size_t background = 0;
        size_t overlay = 0;
        int16_t overlay_spriteNum = 1;
        ST::camera Camera = {0, 0, -1, 1920, 0, 1080};
    };
}

#endif //SLAVIC_TALES_LEVEL_DATA_HPP