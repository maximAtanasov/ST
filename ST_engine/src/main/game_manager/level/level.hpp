#ifndef LVL_DEF
#define LVL_DEF

#include <input_manager/key_definitions.hpp>
#include <defs.hpp>
#include <game_manager/level/entity.hpp>
#include <game_manager/level/text.hpp>
#include <game_manager/level/light.hpp>
#include <message_bus/message_bus.hpp>
#include <game_manager/level/camera.hpp>

typedef struct level_data{
    std::unordered_map<size_t, key> actions_Buttons;
    std::vector<entity> entities;
    std::vector<light> lights;
    std::vector<text> text_objects;
    ST::camera Camera = {0, 0, -1, 1920, 0, 1080};
    size_t background;
    size_t overlay = 0;
    int overlay_spriteNum = 1;
} level_data;


#include "../../input_manager/input_manager.hpp"

class level{
    private:
        std::string name;
        level_data data;
        message_bus* gMessage_bus;
        int load_input_conf();
        key key_index(std::string arg);

    public:
        level(const std::string&, message_bus*);
        void load();
        void unload();
        inline level_data* get_data(){
            return &data;
        }
        std::string get_name();
        ~level();
};

#endif
