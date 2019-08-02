/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#include <game_manager/game_manager.hpp>
#include <algorithm>
#include <SDL_events.h>

#define FIRST_LEVEL_NAME "cubes"

static bool singleton_initialized = false;

/**
 * initializes the game_manager and loads the level specified as first ("main" is default)
 * @param msg_bus A pointer to the global message bus.
 * @param tsk_mngr A pointer to the global task_mngr.
 */
game_manager::game_manager(message_bus *msg_bus, task_manager *tsk_mngr){

    if(singleton_initialized){
        throw std::runtime_error("The game manager cannot be initialized more than once!");
    } else {
        singleton_initialized = true;
    }

    gMessage_bus = msg_bus;
    gScript_backend.initialize(gMessage_bus, this);
    gTask_manager = tsk_mngr;

    //subscribe to messages
    gMessage_bus->subscribe(LOAD_LEVEL, &msg_sub);
    gMessage_bus->subscribe(START_LEVEL, &msg_sub);
    gMessage_bus->subscribe(UNLOAD_LEVEL, &msg_sub);
    gMessage_bus->subscribe(RELOAD_LEVEL, &msg_sub);
    gMessage_bus->subscribe(KEY_PRESSED, &msg_sub);
    gMessage_bus->subscribe(KEY_HELD, &msg_sub);
    gMessage_bus->subscribe(KEY_RELEASED, &msg_sub);
    gMessage_bus->subscribe(MOUSE_X, &msg_sub);
    gMessage_bus->subscribe(MOUSE_Y, &msg_sub);
    gMessage_bus->subscribe(LEFT_TRIGGER, &msg_sub);
    gMessage_bus->subscribe(RIGHT_TRIGGER, &msg_sub);
    gMessage_bus->subscribe(LEFT_STICK_HORIZONTAL, &msg_sub);
    gMessage_bus->subscribe(LEFT_STICK_VERTICAL, &msg_sub);
    gMessage_bus->subscribe(RIGHT_STICK_HORIZONTAL, &msg_sub);
    gMessage_bus->subscribe(RIGHT_STICK_VERTICAL, &msg_sub);
    gMessage_bus->subscribe(VSYNC_STATE, &msg_sub);
    gMessage_bus->subscribe(END_GAME, &msg_sub);
    gMessage_bus->subscribe(MUSIC_VOLUME_LEVEL, &msg_sub);
    gMessage_bus->subscribe(SOUNDS_VOLUME_LEVEL, &msg_sub);
    gMessage_bus->subscribe(SHOW_MOUSE, &msg_sub);
    gMessage_bus->subscribe(EXECUTE_SCRIPT, &msg_sub);
    gMessage_bus->subscribe(FULLSCREEN_STATUS, &msg_sub);
    gMessage_bus->subscribe(AUDIO_ENABLED, &msg_sub);
    gMessage_bus->subscribe(VIRTUAL_SCREEN_COORDINATES, &msg_sub);

    //initialize initial states of keys
    reset_keys();

    //Load the first level
    load_level(FIRST_LEVEL_NAME);
    start_level(FIRST_LEVEL_NAME);
    game_is_running_ = true;
}

/**
 * Reset all information about input.
 */
void game_manager::reset_keys(){
    keys_pressed_data.reset();
    keys_held_data.reset();
    keys_released_data.reset();
}

/**
 * Consumes message from the subscriber object and performs the appropriate actions.
 */
void game_manager::handle_messages(){
    message* temp = msg_sub.get_next_message();
    while(temp != nullptr){
        if(temp->msg_name == LOAD_LEVEL){
            load_level(*static_cast<std::string*>(temp->get_data()));
        }
        else if(temp->msg_name == RELOAD_LEVEL){
            reload_level(*static_cast<std::string*>(temp->get_data()));
        }
        else if(temp->msg_name == START_LEVEL){
            start_level(*static_cast<std::string*>(temp->get_data()));
        }
        else if(temp->msg_name == UNLOAD_LEVEL){
            unload_level(*static_cast<std::string*>(temp->get_data()));
        }
        else if(temp->msg_name == KEY_PRESSED){
            uint8_t key_index = temp->base_data0;
            keys_pressed_data[key_index] = true;
            keys_held_data[key_index] = false;
            keys_released_data[key_index] = false;
        }
        else if(temp->msg_name == KEY_HELD){
            uint8_t key_index = temp->base_data0;
            auto key_val = static_cast<ST::key>(temp->base_data0);
            keys_pressed_data[key_index] = false;
            keys_held_data[key_index] = true;
            keys_released_data[key_index] = false;
        }
        else if(temp->msg_name == KEY_RELEASED){
            uint8_t key_index = temp->base_data0;
            keys_pressed_data[key_index] = false;
            keys_held_data[key_index] = false;
            keys_released_data[key_index] = true;
        }
        else if(temp->msg_name == MOUSE_X){
            mouse_x = static_cast<int32_t>(temp->base_data0);
        }
        else if(temp->msg_name == MOUSE_Y){
            mouse_y = static_cast<int32_t>(temp->base_data0);
        }
        else if(temp->msg_name == LEFT_TRIGGER){
            left_trigger = static_cast<int16_t>(temp->base_data0);
        }
        else if(temp->msg_name == RIGHT_TRIGGER){
            right_trigger = static_cast<int16_t>(temp->base_data0);
        }
        else if(temp->msg_name == LEFT_STICK_VERTICAL){
            left_stick_vertical = static_cast<int16_t>(temp->base_data0);
        }
        else if(temp->msg_name == LEFT_STICK_HORIZONTAL){
            left_stick_horizontal = static_cast<int16_t>(temp->base_data0);
        }
        else if(temp->msg_name == RIGHT_STICK_VERTICAL){
            right_stick_vertical = static_cast<int16_t>(temp->base_data0);
        }
        else if(temp->msg_name == RIGHT_STICK_HORIZONTAL){
            right_stick_horizontal = static_cast<int16_t>(temp->base_data0);
        }
        else if(temp->msg_name == MUSIC_VOLUME_LEVEL){
            music_volume_level = static_cast<uint8_t>(temp->base_data0);
        }
        else if(temp->msg_name == SOUNDS_VOLUME_LEVEL){
            sounds_volume_level = static_cast<uint8_t>(temp->base_data0);
        }
        else if(temp->msg_name == AUDIO_ENABLED){
            audio_enabled = static_cast<bool>(temp->base_data0);
        }
        else if(temp->msg_name == VSYNC_STATE){
            vsync_flag = static_cast<bool>(temp->base_data0);
        }
        else if(temp->msg_name == END_GAME){
            game_is_running_ = false;
        }
        else if(temp->msg_name == SHOW_MOUSE){
            SDL_ShowCursor(static_cast<bool>(temp->base_data0));
        }
        else if(temp->msg_name == EXECUTE_SCRIPT){
            auto script = static_cast<std::string*>(temp->get_data());
            gScript_backend.run_script(*script);
        }
        else if(temp->msg_name == FULLSCREEN_STATUS){
            fullscreen_status = static_cast<bool>(temp->base_data0);
        }
        else if(temp->msg_name == VIRTUAL_SCREEN_COORDINATES){
            auto data = temp->base_data0;
            v_width = data & 0x0000ffffU;
            v_height = (data >> 16U) & 0x0000ffffU;
        }
        delete temp;
        temp = msg_sub.get_next_message();
    }
}

/**
 * Loads a level if it is not already loaded.
 * @param level_name The name of the level to load (this must the name of the folder).
 */
int8_t game_manager::load_level(const std::string& level_name){

    //check if it is already loaded.
    for(auto& i: levels) {
        if (i.get_name() == level_name) {
            return 0;
        }
    }

    //otherwise - create it
    auto temp = ST::level(level_name, gMessage_bus);
    if(temp.load() != 0){
        gMessage_bus->send_msg(new message(LOG_ERROR, make_data<std::string>("Level with name " + level_name + " could not be loaded!")));
        return -1;
    }
    levels.emplace_back(temp);

    //current level pointer must be reset, because apparently adding to the vector changes the pointer address
    //(makes sense as it has to reallocate the whole thing)
    for (auto &level : levels) {
        if(level.get_name() == active_level){
            current_level_pointer = &level;
            break;
        }
    }
    return 0;
}

/**
 * Unloads a level.
 * @param level_name The name of the level to unload (this must the name of the folder).
 */
void game_manager::unload_level(const std::string& level_name){
    for(Uint32 i = 0; i < levels.size(); ++i) {
        if (levels[i].get_name() == level_name) {
            levels[i].unload();
            levels.erase(levels.begin() + i);
            break;
        }
    }

    //current level pointer must be reset, because apparently adding/removing to/from the vector changes the pointer address
    //(makes sense as it has to reallocate the whole thing)
    for (auto &level : levels) {
        if(level.get_name() == active_level) {
            current_level_pointer = &level;
            break;
        }
    }
}


/**
 * Reloads a level (works while the level is being run).
 * @param level_name The name of the level to unload (this must the name of the folder).
 */
void game_manager::reload_level(const std::string& level_name){
    for (auto &level : levels) {
        if (level.get_name() == level_name) {
            level.reload();
            break;
        }
    }
}


/**
 * Starts a level given it's name.
 * @param level_name The name of the level to start (this must the name of the folder).
 */
void game_manager::start_level(const std::string& level_name) {

    //set the current level pointer
    bool is_loaded = false;
    for (auto &level : levels) {
        if(level.get_name() == level_name) {
            current_level_pointer = &level;
            is_loaded = true;
            break;
        }
    }
    //if the level wasn't loaded in advance, load it now
    if(!is_loaded){
        if(load_level(level_name) == 0) {
            for (auto &level : levels) {
                if (level.get_name() == level_name) {
                    current_level_pointer = &level;
                    break;
                }
            }
        } else {
            gMessage_bus->send_msg(new message(LOG_ERROR, make_data<std::string>("Error starting level " + level_name)));
            return;
        }
    }

    gScript_backend.close();
    gScript_backend.initialize(gMessage_bus, this);
    active_level = level_name;

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
}

/**
 * Closes the game manager and the lua backend.
 */
game_manager::~game_manager(){
    gScript_backend.close();
    singleton_initialized = false;
}


//TODO: Docs
/**
 * @return The value of the left trigger button on a controller
 */
int16_t game_manager::get_left_trigger() const {
    return left_trigger;
}

/**
 * @return The value of the right trigger button on a controller
 */
int16_t game_manager::get_right_trigger() const {
    return right_trigger;
}

int16_t game_manager::get_left_stick_horizontal() const {
    return left_stick_horizontal;
}

int16_t game_manager::get_left_stick_vertical() const {
    return left_stick_vertical;
}

int16_t game_manager::get_right_stick_vertical() const {
    return right_stick_vertical;
}

int16_t game_manager::get_right_stick_horizontal() const {
    return right_stick_horizontal;
}
