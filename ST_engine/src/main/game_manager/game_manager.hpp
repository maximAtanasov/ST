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
class game_manager {
private:

    std::vector<ST::level> levels{};
    std::string active_level{};
    ST::level *current_level_pointer{};
    subscriber msg_sub{};
    std::atomic_bool game_is_running_{};
    lua_backend gScript_backend{};
    message_bus &gMessage_bus;
    task_manager *gTask_manager{};
    std::bitset<256> keys_pressed_data{};
    std::bitset<256> keys_held_data{};
    std::bitset<256> keys_released_data{};
    int32_t mouse_x = 0;
    int32_t mouse_y = 0;
    int16_t left_trigger{};
    int16_t right_trigger{};
    int16_t left_stick_horizontal{};
    int16_t left_stick_vertical{};
    int16_t right_stick_vertical{};
    int16_t right_stick_horizontal{};

    //Virtual screen size
    uint16_t v_width = 1920;
    uint16_t v_height = 1080;

    //Window screen size
    uint16_t w_width = 1920;
    uint16_t w_height = 1080;

    //methods
    void handle_messages();

    int8_t load_level(const std::string &);

    void unload_level(const std::string &);

    void reload_level(const std::string &);

    void start_level(const std::string &);

    void reset_keys();

    void run_level_loop();

public:
    bool vsync_flag = true;
    bool fullscreen_status = false;
    uint8_t music_volume_level = 100;
    uint8_t sounds_volume_level = 100;
    bool audio_enabled = true;
    int8_t gravity = 0;

    explicit game_manager(message_bus &msg_bus);

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

    [[nodiscard]] ST::level *get_level() const;

    void center_camera_on_entity(uint64_t id);

    uint16_t get_window_width() const;

    uint16_t get_window_height() const;

    uint16_t get_internal_width() const;

    uint16_t get_internal_height() const;

    void save_state(const std::string &filepath);
};

#endif /*ST_GAME_MANAGER_DEF*/