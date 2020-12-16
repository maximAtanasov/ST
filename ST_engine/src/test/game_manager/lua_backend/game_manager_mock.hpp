/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#ifndef ST_GAME_MANAGER_MOCK_HPP
#define ST_GAME_MANAGER_MOCK_HPP

#include <vector>
#include <SDL_atomic.h>
#include <task_manager.hpp>
#include "../../../../ST_message_bus/include/message_bus.hpp"
#include <game_manager/level/level.hpp>

/**
 *
 * MOCK:
 * contained in game_manager_mock.hpp
 * This is a mock class of the game_manager.
 * It contains the exact same function definitions as the real game_manager, but not the implementations.
 * This is need to properly test the lua_backend.
 * GMock is not used as it requires to use virtual methods (interfaces) to create mocks.
 * We assume that this mock contains the same definitions as the real thing, but this is not checked anywhere and
 * is done per hand. (A compile time error will occur if a method/attribute is not found, but things like wrong data types in
 * the method arguments may not be noticed easily).
 */
class game_manager{
private:

    //methods
    void handle_messages();
    int8_t load_level(const std::string&){return 0;}
    void unload_level(const std::string&){}
    void start_level(const std::string&){}
    void reset_keys(){}
    void run_level_loop(){}

    ST::level* level = new ST::level("test_level", nullptr);

public:

    bool fullscreen_status = false;
    bool vsync_flag = true;
    uint8_t music_volume_level = 100;
    uint8_t sounds_volume_level = 100;
    bool audio_enabled = true;
    int8_t gravity = 0;

    game_manager(message_bus* msg_bus, task_manager* tsk_mngr){}
    ~game_manager() = default;
    std::string get_active_level() const {return "test_level";}

    ST::level* get_level() {
        get_level_calls++;
        return level;
    }

    bool key_pressed(size_t arg) const { return true;};
    bool key_held(size_t arg) const {return true;};
    bool key_released(size_t arg) const {return true;};
    int32_t get_mouse_x() const {return 100;};
    int32_t get_mouse_y() const {return 200;};
    void update() {};
    bool game_is_running() const { return true; };
    void center_camera_on_entity(uint64_t id){center_camera_on_entity_calls++;}
    int16_t get_left_trigger() const{return 100;}
    int16_t get_right_trigger() const{return 200;}
    int16_t get_left_stick_horizontal() const{return 100;}
    int16_t get_left_stick_vertical() const{return 200;}
    int16_t get_right_stick_vertical() const{ return 300;}
    int16_t get_right_stick_horizontal() const{return 400;}
    void save_state(const std::string& filepath) {};

    //Variables to keep track of method calls
    uint8_t get_level_calls = 0;
    uint8_t center_camera_on_entity_calls = 0;
 };


#endif //ST_GAME_MANAGER_MOCK_HPP
