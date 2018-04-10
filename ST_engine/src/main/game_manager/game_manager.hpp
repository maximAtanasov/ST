#ifndef LVL_MNG_DEF
#define LVL_MNG_DEF

#include <defs.hpp>
#include <game_manager/level/level.hpp>
#include <message_bus/message_bus.hpp>
#include <game_manager/lua_backend/lua_backend.hpp>

class game_manager{
    private:
        std::vector<level*> levels{};
        std::string active_level{};
        level* current_level_pointer{};
        subscriber* msg_sub{};
        SDL_atomic_t end_game{};
        lua_backend* gScript_backend{};

        message_bus* gMessage_bus{};
        task_manager* gTask_manager{};
        bool keys_pressed_data[64]{};
        bool keys_held_data[64]{};
        bool keys_released_data[64]{};
        int mouse_x = 0;
        int mouse_y = 0;

        task_id update_id{};

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
        int volume_level = 0;

        game_manager() = default;
        std::string get_active_level();
        level_data* get_level_data();
        bool key_pressed(size_t arg);
        bool key_held(size_t arg);
        bool key_released(size_t arg);
        int get_mouse_x();
        int get_mouse_y();
        int initialize(message_bus* msg_bus, task_manager* tsk_mngr);
        void update();
        void close();
        bool game_is_running();
		level get_level();
};

//INLINED METHODS

inline void game_manager::update(){
    handle_messages();
    run_level_loop();
}

inline std::string game_manager::get_active_level(){
    return active_level;
}

inline void game_manager::close(){
    handle_messages();
    delete msg_sub;
    gScript_backend->close();
}

inline void game_manager::run_level_loop() {
    gScript_backend->run_global("loop");
}

inline level_data* game_manager::get_level_data() {
    return current_level_pointer->get_data();
}

inline level game_manager::get_level(){
    return *current_level_pointer;
}

inline bool game_manager::game_is_running(){
    return (bool)SDL_AtomicGet(&end_game);
}

inline int game_manager::get_mouse_x(){
    return mouse_x;
}

inline int game_manager::get_mouse_y(){
    return mouse_y;
}


inline bool game_manager::key_pressed(size_t arg){
    return keys_pressed_data[(Uint8)(get_level_data()->actions_Buttons[arg])];
}

inline bool game_manager::key_held(size_t arg){
    return keys_held_data[(Uint8)get_level_data()->actions_Buttons[arg]];
}

inline bool game_manager::key_released(size_t arg){
    return keys_released_data[(Uint8)get_level_data()->actions_Buttons[arg]];
}


#endif