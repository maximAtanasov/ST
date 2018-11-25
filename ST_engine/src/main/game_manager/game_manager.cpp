/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <game_manager/game_manager.hpp>
#include <algorithm>
#include <main/message_types.hpp>

#define FIRST_LEVEL_NAME "main"

/**
 * initializes the game_manager and loads the level specified as first ("main" is default)
 * @param msg_bus A pointer to the global message bus.
 * @param tsk_mngr A pointer to the global task_mngr.
 */
game_manager::game_manager(message_bus *msg_bus, task_manager *tsk_mngr){

    gMessage_bus = msg_bus;
    gScript_backend.initialize(gMessage_bus, this);
    gTask_manager = tsk_mngr;

    //subscribe to messages
    gMessage_bus->subscribe(LOAD_LEVEL, &msg_sub);
    gMessage_bus->subscribe(START_LEVEL, &msg_sub);
    gMessage_bus->subscribe(UNLOAD_LEVEL, &msg_sub);
    gMessage_bus->subscribe(KEY_PRESSED, &msg_sub);
    gMessage_bus->subscribe(KEY_HELD, &msg_sub);
    gMessage_bus->subscribe(KEY_RELEASED, &msg_sub);
    gMessage_bus->subscribe(MOUSE_X, &msg_sub);
    gMessage_bus->subscribe(MOUSE_Y, &msg_sub);
    gMessage_bus->subscribe(VSYNC_STATE, &msg_sub);
    gMessage_bus->subscribe(END_GAME, &msg_sub);
    gMessage_bus->subscribe(MUSIC_VOLUME_LEVEL, &msg_sub);
    gMessage_bus->subscribe(SOUNDS_VOLUME_LEVEL, &msg_sub);
    gMessage_bus->subscribe(SHOW_MOUSE, &msg_sub);
    gMessage_bus->subscribe(EXECUTE_SCRIPT, &msg_sub);
    gMessage_bus->subscribe(FULLSCREEN_STATUS, &msg_sub);
    gMessage_bus->subscribe(AUDIO_ENABLED, &msg_sub);
    //initialize initial states of keys
    reset_keys();

    //Load the first level
    load_level(FIRST_LEVEL_NAME);
    start_level(FIRST_LEVEL_NAME);
    SDL_AtomicSet(&end_game, 1);
}

/**
 * Reset all information about input.
 */
void game_manager::reset_keys(){
    memset(keys_pressed_data, false, sizeof(bool) * 64);
    memset(keys_held_data, false, sizeof(bool) * 64);
    memset(keys_released_data, false, sizeof(bool) * 64);
}

/**
 * Consumes message from the subscriber object and performs the appropriate actions.
 */
void game_manager::handle_messages(){
    message* temp = msg_sub.get_next_message();
    while(temp != nullptr){
        if(temp->msg_name == LOAD_LEVEL){
            auto name = static_cast<std::string*>(temp->get_data());
            load_level(*name);
        }
        else if(temp->msg_name == START_LEVEL){
            auto name = static_cast<std::string*>(temp->get_data());
            start_level(*name);
        }
        else if(temp->msg_name == UNLOAD_LEVEL){
            auto name = static_cast<std::string*>(temp->get_data());
            unload_level(*name);
        }
        else if(temp->msg_name == KEY_PRESSED){
            uint8_t key_index = *static_cast<uint8_t*>(temp->get_data());
            keys_pressed_data[key_index] = true;
            keys_held_data[key_index] = false;
            keys_released_data[key_index] = false;
        }
        else if(temp->msg_name == KEY_HELD){
            uint8_t key_index = *static_cast<uint8_t*>(temp->get_data());
            keys_pressed_data[key_index] = false;
            keys_held_data[key_index] = true;
            keys_released_data[key_index] = false;
        }
        else if(temp->msg_name == KEY_RELEASED){
            uint8_t key_index = *static_cast<uint8_t*>(temp->get_data());
            keys_pressed_data[key_index] = false;
            keys_held_data[key_index] = false;
            keys_released_data[key_index] = true;
        }
        else if(temp->msg_name == MOUSE_X){
            int32_t x = *static_cast<int32_t*>(temp->get_data());
            mouse_x = x;
        }
        else if(temp->msg_name == MOUSE_Y){
            int32_t y = *static_cast<int32_t*>(temp->get_data());
            mouse_y = y;
        }
        else if(temp->msg_name == MUSIC_VOLUME_LEVEL){
            uint8_t val = *static_cast<uint8_t*>(temp->get_data());
            music_volume_level = val;
        }
        else if(temp->msg_name == MUSIC_VOLUME_LEVEL){
            uint8_t val = *static_cast<uint8_t*>(temp->get_data());
            sounds_volume_level = val;
        }
        else if(temp->msg_name == AUDIO_ENABLED){
            auto val = static_cast<bool*>(temp->get_data());
            audio_enabled = *val;
        }
        else if(temp->msg_name == VSYNC_STATE){
            auto arg = static_cast<bool*>(temp->get_data());
            vsync_flag = *arg;
        }
        else if(temp->msg_name == END_GAME){
            SDL_AtomicSet(&end_game, 0);
        }
        else if(temp->msg_name == SHOW_MOUSE){
            auto arg = static_cast<bool*>(temp->get_data());
            if(*arg){
                SDL_ShowCursor(SDL_ENABLE);
            }else{
                SDL_ShowCursor(SDL_DISABLE);
            }
        }
        else if(temp->msg_name == EXECUTE_SCRIPT){
            auto scipt = static_cast<std::string*>(temp->get_data());
            gScript_backend.run_script(*scipt);
        }
        else if(temp->msg_name == FULLSCREEN_STATUS){
            bool arg = *static_cast<bool*>(temp->get_data());
            fullscreen_status = arg;
        }
        destroy_msg(temp);
        temp = msg_sub.get_next_message();
    }
}

/**
 * Loads a level if it is not already loaded.
 * @param level_name The name of the level to load (this must the name of the folder).
 */
void game_manager::load_level(const std::string& level_name){

    //check if it is already loaded.
    for(auto& i: levels) {
        if (i.get_name() == level_name) {
            return;
        }
    }

    //otherwise - create it
    auto temp = ST::level(level_name, gMessage_bus);
    temp.load();
    levels.push_back(temp);

    //current level pointer must be reset, because apparently adding to the vector changes the pointer address
    //(makes sense as it has to reallocate the whole thing)
    for (auto &level : levels) {
        if(level.get_name() == active_level){
            current_level_pointer = &level;
            break;
        }
    }
}

/**
 * Unloads a level.
 * @param level_name The name of the level to unload (this must the name of the folder).
 */
void game_manager::unload_level(const std::string& level_name){
    for(Uint32 i = 0; i < levels.size(); i++) {
        if (levels[i].get_name() == level_name) {
            levels[i].unload();
            levels.erase(levels.begin() + i);
            break;
        }
    }

    //current level pointer must be reset, because apparently adding to the vector changes the pointer address
    //(makes sense as it has to reallocate the whole thing)
    for (auto &level : levels) {
        if(level.get_name() == active_level){
            current_level_pointer = &level;
            break;
        }
    }
}

/**
 * Starts a level given it's name. The level must be loaded in advance.
 * @param level_name The name of the level to start (this must the name of the folder).
 */
void game_manager::start_level(const std::string& level_name){
    gScript_backend.close();
    gScript_backend.initialize(gMessage_bus, this);
    active_level = level_name;

    //set current level pointer
    for (auto &level : levels) {
        if(level.get_name() == active_level){
            current_level_pointer = &level;
            break;
        }
    }

    get_level()->lights.clear();
    get_level()->entities.clear();
    get_level()->text_objects.clear();

    //construct level
    get_level()->Camera.x = 0;
    get_level()->Camera.y = 0;

    std::string temp = "levels/";
    temp = temp + active_level;
    temp = temp + "/level.lua";
    gScript_backend.run_file(temp);

    //load level loop
    temp = "levels/";
    temp = temp + active_level;
    temp = temp + "/loop.lua";
    gScript_backend.load_file(temp);
    gScript_backend.set_global("loop");

}

/**
 * Closes the game manager and the lua backend.
 * Consumes any leftover messages.
 */
game_manager::~game_manager(){
    handle_messages();
    gScript_backend.close();
}
