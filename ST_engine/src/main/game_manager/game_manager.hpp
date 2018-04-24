/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#ifndef LVL_MNG_DEF
#define LVL_MNG_DEF

#include <defs.hpp>
#include <game_manager/level/level.hpp>
#include <message_bus/message_bus.hpp>
#include <game_manager/lua_backend/lua_backend.hpp>

///This class is responsible for managing all levels and the lua backend, it is the heart of the engine.
/**
 * Messages this subsystem listens to: <br>
 *
 * <b>LOAD_LEVEL</b> - Loads a level given it's name.
 *
 * Message must contain: a pointer to a <b>std::string</b> containing the name of the level. (the name of it's folder)<br>
 *
 * <b>START_LEVEL</b> - Starts a level given it's name (must be loaded in advance). <br>
 *
 * Message must contain: a pointer to a <b>std::string</b> containing the name of the level. (the name of it's folder)<br>
 *
 * <b>UNLOAD_LEVEL</b> - Unloads a level given it's name. <br>
 *
 * Message must contain: a pointer to a <b>std::string</b> containing the name of the level. (the name of it's folder)<br>
 *
 * <b>KEY_PRESSED</b> - An event signifying that a key is pressed. <br>
 *
 * Message must contain: a pointer to <b>uint8_t</b> representing the key value. See key_definitions.hpp .<br>
 *
 * <b>KEY_HELD</b> - An event signifying that a key is held. <br>
 *
 * Message must contain: a pointer to <b>uint8_t</b> representing the key value. See key_definitions.hpp .<br>
 *
 * <b>KEY_RELEASED</b> - An event signifying that a key is released. <br>
 *
 * Message must contain: a pointer to <b>uint8_t</b> representing the key value. See key_definitions.hpp .<br>
 *
 * <b>MOUSE_X</b> - The current mouse X position. <br>
 *
 * Message must contain: a pointer to a <b>int32_t</b>. <br>
 *
 * <b>MOUSE_Y</b> -The current mouse Y position. <br>
 *
 * Message must contain: a pointer to a <b>int32_t</b>. <br>
 *
 * <b>VOLUME_LEVEL</b> - An event containing the current volume. <br>
 *
 * Message must contain: a pointer to a <b>uint8_t</b>. <br>
 *
 * <b>VSYNC_IS_ON</b> - An event meaning vsync is on <br>
 *
 * Message must contain: a <b>nullptr</b>. <br>
 *
 * <b>VSYNC_IS_OFF</b> - An event meaning vsync is off <br>
 *
 * Message must contain: a <b>nullptr</b>. <br>
 *
 * <b>END_GAME</b> - Recieving this event causes the application to shutdown. <br>
 *
 * Message must contain: a <b>nullptr</b>. <br>
 *
 * <b>SHOW_MOUSE</b> - A boolean message to show the mouse cursor or to hide it. <br>
 *
 * Message must contain: a pointer to a <b>bool</b>. <br>
 *
 * <b>EXECUTE_SCRIPT</b> - Executes a script in the global Lua state. <br>
 *
 * Message must contain: a pointer to a <b>std::string</b> containing script. <br>
 *
 * <b>FULLSCREEN_STATUS</b> - A message containing the current fullscreen status. <br>
 *
 * Message must contain: a pointer to a <b>bool</b>. <br>
 *
 */
class game_manager{
    private:
        std::vector<ST::level> levels{};
        std::string active_level{};
        ST::level* current_level_pointer{};
        subscriber msg_sub{};
        SDL_atomic_t end_game{};
        lua_backend gScript_backend{};

        message_bus* gMessage_bus{};
        task_manager* gTask_manager{};
        bool keys_pressed_data[64]{};
        bool keys_held_data[64]{};
        bool keys_released_data[64]{};
        int32_t mouse_x = 0;
        int32_t mouse_y = 0;

        //methods
        void handle_messages();
        void load_level(const std::string&);
        void unload_level(const std::string&);
        void start_level(const std::string&);
        void reset_keys();
        void run_level_loop();

    public:
        bool vsync_flag = true;
        bool fullscreen_status = false;
        uint8_t volume_level = 0;

        game_manager() = default;
        std::string get_active_level();
        ST::level_data* get_level_data();
        bool key_pressed(size_t arg);
        bool key_held(size_t arg);
        bool key_released(size_t arg);
        int32_t get_mouse_x();
        int32_t get_mouse_y();
        int initialize(message_bus* msg_bus, task_manager* tsk_mngr);
        void update();
        void close();
        bool game_is_running();
        ST::level get_level();
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
inline std::string game_manager::get_active_level(){
    return active_level;
}

/**
 * Closes the game manager and the lua backend.
 * Consumes any leftover messages.
 */
inline void game_manager::close(){
    handle_messages();
    gScript_backend.close();
}

/**
 * Runs one iteration of the global loop script.
 */
inline void game_manager::run_level_loop() {
    gScript_backend.run_global("loop");
}

/**
 * Returns the data of the current level.
 * @return A pointer to the data of the current level.
 */
inline ST::level_data* game_manager::get_level_data() {
    return current_level_pointer->get_data();
}

/**
 * @return A poitner to the current level.
 */
inline ST::level game_manager::get_level(){
    return *current_level_pointer;
}

/**
 * Tells is if the game is still running or not.
 * This is the main condition for running the main loop (the entire engine).
 * @return True if running, false otherwise.
 */
inline bool game_manager::game_is_running(){
    return static_cast<bool>(SDL_AtomicGet(&end_game));
}

/**
 * Get the current mouse X position.
 * @return The X position of the mouse cursor.
 */
inline int game_manager::get_mouse_x(){
    return mouse_x;
}

/**
 * Get the current mouse Y position.
 * @return The Y position of the mouse cursor.
 */
inline int game_manager::get_mouse_y(){
    return mouse_y;
}

/**
 * Tells if a key corresponding to a certain action has been pressed.
 * @param arg A hash of the name of the action.
 * @return True if pressed, false otherwise.
 */
inline bool game_manager::key_pressed(size_t arg){
    return keys_pressed_data[static_cast<uint8_t>(get_level_data()->actions_Buttons[arg])];
}

/**
 * Tells if a key corresponding to a certain action has been held.
 * @param arg A hash of the name of the action.
 * @return True if held, false otherwise.
 */
inline bool game_manager::key_held(size_t arg){
    return keys_held_data[static_cast<uint8_t>(get_level_data()->actions_Buttons[arg])];
}

/**
 * Tells if a key corresponding to a certain action has been released.
 * @param arg A hash of the name of the action.
 * @return True if released, false otherwise.
 */
inline bool game_manager::key_released(size_t arg){
    return keys_released_data[static_cast<uint8_t>(get_level_data()->actions_Buttons[arg])];
}

#endif