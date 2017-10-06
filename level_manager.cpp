#include "lua_manager.hpp"
#include "level_manager.hpp"
#include "level.hpp"

level_manager::level_manager(){
    //empty constructor
}

int level_manager::initialize(){
    return 0;
}

void level_manager::close(void){
    unload_level(active_level);
}

void level_manager::load_level(std::string level_name){
    for(Uint32 i = 0; i < levels.size(); i++)
        if(levels[i].get_name().compare(level_name) == 0){
            return;
        }
    level temp = level(level_name);
    temp.load();
    levels.push_back(temp);

    //current level pointer must be reset, because apparently adding to the vector changes the pointer address
    //(makes sense as it has to reallocate the whole thing)
    for(Uint32 i = 0; i < levels.size(); i++)
        if(levels[i].get_name().compare(active_level) == 0){
            current_level_pointer = &levels[i];
            break;
        }

    gConsole.write("Level " + level_name + " loaded");
    std::cout << "Level " << level_name << " loaded!\n";
}

void level_manager::unload_level(std::string level_name){
    for(Uint32 i = 0; i < levels.size(); i++)
        if(levels[i].get_name().compare(level_name) == 0){
            levels[i].unload();
            levels.erase(levels.begin()+i);
            break;
        }
}

void level_manager::start_level(std::string level_name){
    //level must be loaded with level_load() before it can be started
    //reinitialize lua - must be done, because!
    gLua_manager.initialize();
    active_level = level_name;

    //set current level pointer
    for(Uint32 i = 0; i < levels.size(); i++)
        if(levels[i].get_name().compare(active_level) == 0){
            current_level_pointer = &levels[i];
            break;
        }

    //construct level
    camera.x = 0;
    camera.y = 0;
    std::string temp = "levels/";
    temp = temp + active_level;
    temp = temp + "/level.lua";
    gLua_manager.run_file(temp);

    //load level loop
    temp = "levels/";
    temp = temp + active_level;
    temp = temp + "/loop";

    //check if loop is precompiled
    if(gLua_manager.load_file(temp) == 0)
        gLua_manager.set_global("loop");
    else{
        temp = temp + ".lua";
        gLua_manager.load_file(temp);
        gLua_manager.set_global("loop");
    }
}

std::string level_manager::get_active_level(){
    return active_level;
}
