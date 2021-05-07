/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#ifndef ST_GAME_MANAGER_DEF
#define ST_GAME_MANAGER_DEF

#define FIRST_LEVEL_NAME "intro_splash"

#include <game_manager/level/level.hpp>
#include <message_bus.hpp>
#include <game_manager/lua_backend/lua_backend.hpp>
#include <task_manager.hpp>
#include <bitset>

///This class is responsible for managing all levels and the lua backend, it is the heart of the engine.
class game_manager{
    private:

        std::vector<ST::level> levels{};
        std::string active_level{};
        ST::level* current_level_pointer{};
        subscriber msg_sub{};
        std::atomic_bool game_is_running_{};
        lua_backend gScript_backend{};
        message_bus& gMessage_bus;
        task_manager* gTask_manager{};
        std::bitset<80> keys_pressed_data{};
        std::bitset<80> keys_held_data{};
        std::bitset<80> keys_released_data{};
        int32_t mouse_x = 0;
        int32_t mouse_y = 0;
        int16_t left_trigger{};
        int16_t right_trigger{};
        int16_t left_stick_horizontal{};
        int16_t left_stick_vertical{};
        int16_t right_stick_vertical{};
        int16_t right_stick_horizontal{};

        int16_t v_width = 1920;
        int16_t v_height = 1080;


    //methods
        void handle_messages();
        int8_t load_level(const std::string&);
        void unload_level(const std::string&);
        void reload_level(const std::string&);
        void start_level(const std::string&);
        void reset_keys();
        void run_level_loop();

    public:
        bool vsync_flag = true;
        bool fullscreen_status = false;
        uint8_t music_volume_level = 100;
        uint8_t sounds_volume_level = 100;
        bool audio_enabled = true;
        int8_t gravity = 0;

        explicit game_manager(message_bus& msg_bus);
        ~game_manager();
        [[nodiscard]] std::string get_active_level() const;
        [[nodiscard]] bool key_pressed(uint16_t arg) const;
        [[nodiscard]] bool key_held(uint16_t arg) const;
        [[nodiscard]] bool key_released(uint16_t arg) const;
        [[nodiscard]] int32_t get_mouse_x() const;
        [[nodiscard]] int32_t get_mouse_y() const;
        [[nodiscard]] int16_t get_left_trigger() const;
        [[nodiscard]] int16_t get_right_trigger() const;
        [[nodiscard]] int16_t get_left_stick_horizontal() const;
        [[nodiscard]] int16_t get_left_stick_vertical() const;
        [[nodiscard]] int16_t get_right_stick_vertical() const;
        [[nodiscard]] int16_t get_right_stick_horizontal() const;
        void update();
        [[nodiscard]] bool game_is_running() const;
        [[nodiscard]] ST::level* get_level() const;
        void center_camera_on_entity(uint64_t id);
        void save_state(const std::string& filepath);
};

//INLINED METHODS

/**
 * Listens to messages and runs one iteration of loop.lua for the specific level.
 */
inline void game_manager::update(){
    handle_messages();
    run_level_loop();
}

/**
 * Get the name of the active level.
 * @return The name of the current level.
 */
inline std::string game_manager::get_active_level() const{
    return active_level;
}

/**
 * Runs one iteration of the global loop script.
 */
inline void game_manager::run_level_loop() {
    gScript_backend.run_global("loop");
}

/**
 * @return A pointer to the current level.
 */
inline ST::level* game_manager::get_level() const{
    return current_level_pointer;
}

/**
 * Tells is if the game is still running or not.
 * This is the main condition for running the main loop (the entire engine).
 * @return True if running, false otherwise.
 */
inline bool game_manager::game_is_running() const{
    return game_is_running_;
}

/**
 * Get the current mouse X position.
 * @return The X position of the mouse cursor.
 */
inline int32_t game_manager::get_mouse_x() const{
    return mouse_x;
}

/**
 * Get the current mouse Y position.
 * @return The Y position of the mouse cursor.
 */
inline int32_t game_manager::get_mouse_y() const{
    return mouse_y;
}

/**
 * Tells if a key corresponding to a certain action has been pressed.
 * @param arg A hash of the name of the action.
 * @return True if pressed, false otherwise.
 */
inline bool game_manager::key_pressed(uint16_t arg) const{
    for(ST::key key : current_level_pointer->actions_buttons[arg]){
        if(keys_pressed_data[static_cast<uint8_t>(key)]){
            return true;
        }
    }
    return false;
}

/**
 * Tells if a key corresponding to a certain action has been held.
 * @param arg A hash of the name of the action.
 * @return True if held, false otherwise.
 */
inline bool game_manager::key_held(uint16_t arg) const{
    for(ST::key key : current_level_pointer->actions_buttons[arg]){
        if(keys_held_data[static_cast<uint8_t>(key)]){
            return true;
        }
    }
    return false;
}

/**
 * Tells if a key corresponding to a certain action has been released.
 * @param arg A hash of the name of the action.
 * @return True if released, false otherwise.
 */
inline bool game_manager::key_released(uint16_t arg) const{
    for(ST::key key : current_level_pointer->actions_buttons[arg]){
        if(keys_released_data[static_cast<uint8_t>(key)]){
            return true;
        }
    }
    return false;
}

/**
 * Center the camera on an entity.
 * @param id The ID of the entity to center on.
 */
inline void game_manager::center_camera_on_entity(uint64_t id) { //TODO: Get rid of hardcoded values
    current_level_pointer->camera.x = current_level_pointer->entities[id].x - v_width / 4;
    current_level_pointer->camera.y = current_level_pointer->entities[id].y - v_height;

    uint8_t limit_x1_check = current_level_pointer->camera.x < current_level_pointer->camera.limitX1 + 1;
    uint8_t limit_x2_check = current_level_pointer->camera.x > current_level_pointer->camera.limitX2 - 1;
    uint8_t limit_y1_check = current_level_pointer->camera.y < current_level_pointer->camera.limitY1 + 1;
    uint8_t limit_y2_check = current_level_pointer->camera.y > current_level_pointer->camera.limitY2 - 1;

    current_level_pointer->camera.x =
            limit_x1_check * (current_level_pointer->camera.limitX1 + 1) +
            !limit_x1_check * current_level_pointer->camera.x;
    current_level_pointer->camera.x =
            limit_x2_check * (current_level_pointer->camera.limitX2 - 1) +
            !limit_x2_check * current_level_pointer->camera.x;
    current_level_pointer->camera.y =
            limit_y1_check*(current_level_pointer->camera.limitY1 + 1) +
            !limit_y1_check*current_level_pointer->camera.y;
    current_level_pointer->camera.y =
            limit_y2_check * (current_level_pointer->camera.limitY2 - 1) +
            !limit_y2_check * current_level_pointer->camera.y;
}

#endif /*ST_GAME_MANAGER_DEF*/