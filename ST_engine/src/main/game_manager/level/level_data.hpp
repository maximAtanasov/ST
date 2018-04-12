#ifndef SLAVIC_TALES_LEVEL_DATA_HPP
#define SLAVIC_TALES_LEVEL_DATA_HPP

#include <input_manager/key_definitions.hpp>
#include <game_manager/level/entity.hpp>
#include <game_manager/level/light.hpp>
#include <game_manager/level/text.hpp>
#include <game_manager/level/camera.hpp>

namespace ST {
    struct level_data {
        std::unordered_map<size_t, key> actions_Buttons{};
        std::vector<entity> entities{};
        std::vector<ST::light> lights{};
        std::vector<ST::text> text_objects{};
        ST::camera Camera = {0, 0, -1, 1920, 0, 1080};
        size_t background{};
        size_t overlay = 0;
        int overlay_spriteNum = 1;
    };
}

#endif //SLAVIC_TALES_LEVEL_DATA_HPP