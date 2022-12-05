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
#include <fstream>


static bool singleton_initialized = false;

/**
 * initializes the game_manager and loads the level specified as first ("main" is default)
 * @param msg_bus A pointer to the global message bus.
 * @param tsk_mngr A pointer to the global task_mngr.
 */
game_manager::game_manager(message_bus &gMessageBus) : gMessage_bus(gMessageBus) {

    if (singleton_initialized) {
        throw std::runtime_error("The game manager cannot be initialized more than once!");
    } else {
        singleton_initialized = true;
    }

    gScript_backend.initialize(&gMessage_bus, this);

    //subscribe to messages
    gMessage_bus.subscribe(LOAD_LEVEL, &msg_sub);
    gMessage_bus.subscribe(START_LEVEL, &msg_sub);
    gMessage_bus.subscribe(UNLOAD_LEVEL, &msg_sub);
    gMessage_bus.subscribe(RELOAD_LEVEL, &msg_sub);
    gMessage_bus.subscribe(KEY_PRESSED, &msg_sub);
    gMessage_bus.subscribe(KEY_HELD, &msg_sub);
    gMessage_bus.subscribe(KEY_RELEASED, &msg_sub);
    gMessage_bus.subscribe(MOUSE_X, &msg_sub);
    gMessage_bus.subscribe(MOUSE_Y, &msg_sub);
    gMessage_bus.subscribe(LEFT_TRIGGER, &msg_sub);
    gMessage_bus.subscribe(RIGHT_TRIGGER, &msg_sub);
    gMessage_bus.subscribe(LEFT_STICK_HORIZONTAL, &msg_sub);
    gMessage_bus.subscribe(LEFT_STICK_VERTICAL, &msg_sub);
    gMessage_bus.subscribe(RIGHT_STICK_HORIZONTAL, &msg_sub);
    gMessage_bus.subscribe(RIGHT_STICK_VERTICAL, &msg_sub);
    gMessage_bus.subscribe(VSYNC_STATE, &msg_sub);
    gMessage_bus.subscribe(END_GAME, &msg_sub);
    gMessage_bus.subscribe(MUSIC_VOLUME_LEVEL, &msg_sub);
    gMessage_bus.subscribe(SOUNDS_VOLUME_LEVEL, &msg_sub);
    gMessage_bus.subscribe(SHOW_MOUSE, &msg_sub);
    gMessage_bus.subscribe(EXECUTE_SCRIPT, &msg_sub);
    gMessage_bus.subscribe(FULLSCREEN_STATUS, &msg_sub);
    gMessage_bus.subscribe(AUDIO_ENABLED, &msg_sub);
    gMessage_bus.subscribe(VIRTUAL_SCREEN_COORDINATES, &msg_sub);
    gMessage_bus.subscribe(REAL_SCREEN_COORDINATES, &msg_sub);

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
void game_manager::reset_keys() {
    keys_pressed_data.reset();
    keys_held_data.reset();
    keys_released_data.reset();
}

/**
 * Consumes message from the subscriber object and performs the appropriate actions.
 */
void game_manager::handle_messages() {
    auto temp = msg_sub.get_next_message();
    while (temp != nullptr) {
        switch (temp->msg_name) {
            case LOAD_LEVEL:
                load_level(*static_cast<std::string *>(temp->get_data()));
                break;
            case RELOAD_LEVEL:
                reload_level(*static_cast<std::string *>(temp->get_data()));
                break;
            case START_LEVEL:
                start_level(*static_cast<std::string *>(temp->get_data()));
                break;
            case UNLOAD_LEVEL:
                unload_level(*static_cast<std::string *>(temp->get_data()));
                break;
            case KEY_PRESSED: {
                uint8_t key_index = temp->base_data0;
                keys_pressed_data[key_index] = true;
                keys_held_data[key_index] = false;
                keys_released_data[key_index] = false;
                break;
            }
            case KEY_HELD: {
                uint8_t key_index = temp->base_data0;
                keys_pressed_data[key_index] = false;
                keys_held_data[key_index] = true;
                keys_released_data[key_index] = false;
                break;
            }
            case KEY_RELEASED: {
                uint8_t key_index = temp->base_data0;
                keys_pressed_data[key_index] = false;
                keys_held_data[key_index] = false;
                keys_released_data[key_index] = true;
                break;
            }
            case MOUSE_X:
                mouse_x = static_cast<int32_t>(temp->base_data0);
                break;
            case MOUSE_Y:
                mouse_y = static_cast<int32_t>(temp->base_data0);
                break;
            case LEFT_TRIGGER:
                left_trigger = static_cast<int16_t>(temp->base_data0);
                break;
            case RIGHT_TRIGGER:
                right_trigger = static_cast<int16_t>(temp->base_data0);
                break;
            case LEFT_STICK_VERTICAL:
                left_stick_vertical = static_cast<int16_t>(temp->base_data0);
                break;
            case LEFT_STICK_HORIZONTAL:
                left_stick_horizontal = static_cast<int16_t>(temp->base_data0);
                break;
            case RIGHT_STICK_VERTICAL:
                right_stick_vertical = static_cast<int16_t>(temp->base_data0);
                break;
            case RIGHT_STICK_HORIZONTAL:
                right_stick_horizontal = static_cast<int16_t>(temp->base_data0);
                break;
            case MUSIC_VOLUME_LEVEL:
                music_volume_level = static_cast<uint8_t>(temp->base_data0);
                break;
            case SOUNDS_VOLUME_LEVEL:
                sounds_volume_level = static_cast<uint8_t>(temp->base_data0);
                break;
            case AUDIO_ENABLED:
                audio_enabled = static_cast<bool>(temp->base_data0);
                break;
            case VSYNC_STATE:
                vsync_flag = static_cast<bool>(temp->base_data0);
                break;
            case END_GAME:
                game_is_running_ = false;
                break;
            case SHOW_MOUSE:
                SDL_ShowCursor(static_cast<bool>(temp->base_data0));
                break;
            case EXECUTE_SCRIPT: {
                auto script = static_cast<std::string *>(temp->get_data());
                gScript_backend.run_script(*script);
                break;
            }
            case FULLSCREEN_STATUS:
                fullscreen_status = static_cast<bool>(temp->base_data0);
                break;
            case VIRTUAL_SCREEN_COORDINATES: {
                auto data = temp->base_data0;
                v_width = data & 0x0000ffffU;
                v_height = (data >> 16U) & 0x0000ffffU;
                break;
            }
            case REAL_SCREEN_COORDINATES: {
                auto data = temp->base_data0;
                w_width = data & 0x0000ffffU;
                w_height = (data >> 16U) & 0x0000ffffU;
                break;
            }
        }
        delete temp;
        temp = msg_sub.get_next_message();
    }
}

/**
 * Loads a level if it is not already loaded.
 * @param level_name The name of the level to load (this must the name of the folder).
 */
int8_t game_manager::load_level(const std::string &level_name) {

    //check if it is already loaded.
    for (auto &i: levels) {
        if (i.get_name() == level_name) {
            return 0;
        }
    }

    //otherwise - create it
    auto temp = ST::level(level_name, &gMessage_bus);
    if (temp.load() != 0) {
        gMessage_bus.send_msg(new message(LOG_ERROR, make_data<std::string>(
                "Level with name " + level_name + " could not be loaded!")));
        return -1;
    }
    levels.emplace_back(temp);

    //current level pointer must be reset, because apparently adding to the vector changes the pointer address
    //(makes sense as it has to reallocate the whole thing)
    for (auto &level: levels) {
        if (level.get_name() == active_level) {
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
void game_manager::unload_level(const std::string &level_name) {
    for (uint64_t i = 0; i < levels.size(); ++i) {
        if (levels[i].get_name() == level_name) {
            levels[i].unload();
            levels.erase(levels.begin() + i);
            break;
        }
    }

    //current level pointer must be reset, because apparently adding/removing to/from the vector changes the pointer address
    //(makes sense as it has to reallocate the whole thing)
    for (auto &level: levels) {
        if (level.get_name() == active_level) {
            current_level_pointer = &level;
            break;
        }
    }
}


/**
 * Reloads a level (works while the level is being run).
 * @param level_name The name of the level to unload (this must the name of the folder).
 */
void game_manager::reload_level(const std::string &level_name) {
    for (auto &level: levels) {
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
void game_manager::start_level(const std::string &level_name) {

    //set the current level pointer
    bool is_loaded = false;
    for (auto &level: levels) {
        if (level.get_name() == level_name) {
            current_level_pointer = &level;
            is_loaded = true;
            break;
        }
    }
    //if the level wasn't loaded in advance, load it now
    if (!is_loaded) {
        if (load_level(level_name) == 0) {
            for (auto &level: levels) {
                if (level.get_name() == level_name) {
                    current_level_pointer = &level;
                    break;
                }
            }
        } else {
            gMessage_bus.send_msg(new message(LOG_ERROR, make_data<std::string>("Error starting level " + level_name)));
            return;
        }
    }

    gScript_backend.close();
    gScript_backend.initialize(&gMessage_bus, this);
    active_level = level_name;

    current_level_pointer->lights.clear();
    current_level_pointer->entities.clear();
    current_level_pointer->text_objects.clear();

    //construct level
    current_level_pointer->camera.x = 0;
    current_level_pointer->camera.y = 0;
    current_level_pointer->camera.limitX2 = v_width;
    current_level_pointer->camera.limitY2 = v_height;

    std::string temp = "levels/";
    temp = temp + active_level;
    temp = temp + "/level.lua";
    gScript_backend.run_file(temp);
}

/**
 * Closes the game manager and the lua backend.
 */
game_manager::~game_manager() {
    gScript_backend.close();
    singleton_initialized = false;
}


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

/**
 * @return The value of the left analog stick horizontal axis on a controller
 */
int16_t game_manager::get_left_stick_horizontal() const {
    return left_stick_horizontal;
}

/**
 * @return The value of the left analog stick vertical axis on a controller
 */
int16_t game_manager::get_left_stick_vertical() const {
    return left_stick_vertical;
}

/**
 * @return The value of the right analog stick vertical axis on a controller
 */
int16_t game_manager::get_right_stick_vertical() const {
    return right_stick_vertical;
}

/**
 * @return The value of the right analog stick horizontal axis on a controller
 */
int16_t game_manager::get_right_stick_horizontal() const {
    return right_stick_horizontal;
}

/**
 * Center the camera on an entity.
 * @param id The ID of the entity to center on.
 */
void game_manager::center_camera_on_entity(uint64_t id) {
    //TODO: Get rid of hardcoded values
    //TODO: Move this functionality to lua
    //TODO: Refactor
    if (current_level_pointer->entities[id].velocity_x > 0) {
        if (current_level_pointer->camera.x <
            current_level_pointer->entities[id].x - v_width / 4 - current_level_pointer->entities[id].velocity_x) {
            current_level_pointer->camera.x += current_level_pointer->entities[id].velocity_x + 5;
        } else {
            current_level_pointer->camera.x = current_level_pointer->entities[id].x - v_width / 4;
        }
    } else if (current_level_pointer->entities[id].velocity_x < 0) {
        if (current_level_pointer->camera.x >
            current_level_pointer->entities[id].x - v_width / 2 - current_level_pointer->entities[id].velocity_x) {
            current_level_pointer->camera.x += current_level_pointer->entities[id].velocity_x - 5;
        } else {
            current_level_pointer->camera.x = current_level_pointer->entities[id].x - v_width / 2;
        }
    }
    //TODO: Test different camera set-ups
    //current_level_pointer->camera.x = current_level_pointer->entities[id].x - v_width / 4;

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
            limit_y1_check * (current_level_pointer->camera.limitY1 + 1) +
            !limit_y1_check * current_level_pointer->camera.y;
    current_level_pointer->camera.y =
            limit_y2_check * (current_level_pointer->camera.limitY2 - 1) +
            !limit_y2_check * current_level_pointer->camera.y;
}

/**
 * Listens to messages and runs one iteration of loop.lua for the specific level.
 */
void game_manager::update() {
    handle_messages();
    run_level_loop();
}

/**
 * Get the name of the active level.
 * @return The name of the current level.
 */
std::string game_manager::get_active_level() const {
    return active_level;
}

/**
 * Runs one iteration of the global loop script.
 */
void game_manager::run_level_loop() {
    gScript_backend.run_global("loop");
}

/**
 * @return A pointer to the current level.
 */
ST::level *game_manager::get_level() const {
    return current_level_pointer;
}

/**
 * Tells is if the game is still running or not.
 * This is the main condition for running the main loop (the entire engine).
 * @return True if running, false otherwise.
 */
bool game_manager::game_is_running() const {
    return game_is_running_;
}

/**
 * Get the current mouse X position.
 * @return The X position of the mouse cursor.
 */
int32_t game_manager::get_mouse_x() const {
    return mouse_x;
}

/**
 * Get the current mouse Y position.
 * @return The Y position of the mouse cursor.
 */
int32_t game_manager::get_mouse_y() const {
    return mouse_y;
}

/**
 * Tells if a key corresponding to a certain action has been pressed.
 * @param arg A hash of the name of the action.
 * @return True if pressed, false otherwise.
 */
bool game_manager::key_pressed(uint16_t arg) const {
    for (ST::key key: current_level_pointer->actions_buttons[arg]) {
        if (keys_pressed_data[static_cast<uint8_t>(key)]) {
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
bool game_manager::key_held(uint16_t arg) const {
    for (ST::key key: current_level_pointer->actions_buttons[arg]) {
        if (keys_held_data[static_cast<uint8_t>(key)]) {
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
bool game_manager::key_released(uint16_t arg) const {
    for (ST::key key: current_level_pointer->actions_buttons[arg]) {
        if (keys_released_data[static_cast<uint8_t>(key)]) {
            return true;
        }
    }
    return false;
}

//TODO: Docs
//TODO: Test
void game_manager::save_state(const std::string &filepath) {
    std::ofstream save_file;
    save_file.open(filepath);

    save_file << "L" + current_level_pointer->get_name();
    save_file << "CX" + std::to_string(current_level_pointer->camera.x);
    save_file << "CY" + std::to_string(current_level_pointer->camera.y);

    for (uint8_t i = 0; i < PARALLAX_BG_LAYERS; i++) {
        save_file << "B" + std::to_string(i) + std::to_string(current_level_pointer->background[i]);
    }

    save_file << "BCR" + std::to_string(current_level_pointer->background_color.r) +
                 "G" + std::to_string(current_level_pointer->background_color.g) +
                 "B" + std::to_string(current_level_pointer->background_color.b) +
                 "A" + std::to_string(current_level_pointer->background_color.a);
    save_file << "O" + std::to_string(current_level_pointer->overlay);
    save_file << "OSN" + std::to_string(current_level_pointer->overlay_sprite_num);
    save_file << "E";
    for (const auto &entity: current_level_pointer->entities) {
        save_file << "X" + std::to_string(entity.x);
        save_file << "Y" + std::to_string(entity.y);
        save_file << "VX" + std::to_string(entity.velocity_x);
        save_file << "VY" + std::to_string(entity.velocity_y);
        save_file << "A" + std::to_string(entity.animation);
        save_file << "AN" + std::to_string(entity.animation_num);
        save_file << "SN" + std::to_string(entity.sprite_num);
        save_file << "TH" + std::to_string(entity.tex_h);
        save_file << "TW" + std::to_string(entity.tex_w);
        save_file << "TSX" + std::to_string(entity.tex_scale_x);
        save_file << "TSY" + std::to_string(entity.tex_scale_y);
        save_file << "T" + std::to_string(entity.texture);
        save_file << "S" + std::to_string(entity.is_static());
        save_file << "V" + std::to_string(entity.is_visible());
        save_file << "P" + std::to_string(entity.is_affected_by_physics());
        save_file << "A" + std::to_string(entity.is_active());
    }
    save_file << "E";
    save_file << "L";
    for (const auto &light: current_level_pointer->lights) {
        save_file << "X" + std::to_string(light.origin_x);
        save_file << "Y" + std::to_string(light.origin_y);
        save_file << "S" + std::to_string(light.is_static);
        save_file << "B" + std::to_string(light.brightness);
        save_file << "I" + std::to_string(light.intensity);
        save_file << "R" + std::to_string(light.radius);
    }
    save_file << "L";
    save_file << "T";
    for (const auto &text_object: current_level_pointer->text_objects) {
        save_file << "X" + std::to_string(text_object.x);
        save_file << "Y" + std::to_string(text_object.y);
        save_file << "T" + text_object.text_string;
        save_file << "V" + std::to_string(text_object.is_visible);
        save_file << "F" + std::to_string(text_object.font);
        save_file << "CR" + std::to_string(text_object.color.r) +
                     "G" + std::to_string(text_object.color.g) +
                     "B" + std::to_string(text_object.color.b) +
                     "A" + std::to_string(text_object.color.a);
    }
    save_file << "T";
    save_file.close();
}

/**
 * @return The width of the game window.
 */
uint16_t game_manager::get_window_width() const {
    return w_width;
}

/**
 * @return The height of the game window.
 */
uint16_t game_manager::get_window_height() const {
    return w_height;
}

/**
 * @return The internal rendering width.
 */
uint16_t game_manager::get_internal_width() const {
    return v_width;
}

/**
 * @return The internal rendering height.
 */
uint16_t game_manager::get_internal_height() const {
    return v_height;
}
