#ifndef LVL_MNG_DEF
#define LVL_MNG_DEF

#include "defs.hpp"
#include "level.hpp"
#include "lua_manager.hpp"

class level_manager{
    private:
        std::vector<level> levels;
        std::string active_level;
        level* current_level_pointer;
    public:
        level_manager();
        int initialize();
        void start_level(std::string);
        inline void run_level_loop(){
            applyPhysics();
            gLua_manager.run_global("loop");
        }
        std::string get_active_level();
        void load_level(std::string);
        void unload_level(std::string);
        inline level_data* get_level_data(){
            return current_level_pointer->get_data();
        }
        void close(void);
};

extern level_manager gLevel_manager;

#endif
