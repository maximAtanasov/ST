#ifndef LVL_MNG_DEF
#define LVL_MNG_DEF

#include "defs.hpp"
#include "level.hpp"
#include "manager.hpp"
#include "message_bus.hpp"
#include "lua_backend.hpp"

class game_manager : public virtual manager{
    private:
        std::vector<level*> levels;
        std::string active_level;
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
        void handle_messages() override;
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
        int initialize(message_bus* msg_bus, task_manager* tsk_mngr) override;
        void update() override;
        void close() override;
        bool game_is_running();
		level get_level();
};

#endif