#include "level.hpp"
#include "assets_manager.hpp"



level::level(std::string lvl_name){
    name = lvl_name;
}

void level::load(void){
    load_input_conf();
    std::string temp = "levels/" + name + "/assets.list";
    gAssets_manager.load_assets_from_list(temp);
}

std::string level::get_name(void){
    return name;
}

void level::unload(void){
    //unload fonts
    std::string temp = "levels/" + name;
    temp = temp + "/assets.list";
    gAssets_manager.unload_assets_from_list(temp);

    //unload inputConf
    data.actions_Buttons.clear();

    //unload entities
    for(unsigned int i = 0; i < data.entities.size(); i++)
        delete(data.entities[i]);
        data.entities.clear();
}

int level::load_input_conf(void){
    std::ifstream file;
    std::string temp = "levels/" + name;
    temp = temp + "/inputConf.cfg";
    file.open(temp.c_str());
    if(file.is_open()){
        gConsole.write("Loading " + temp);
        std::cout << "Loading " << temp << "\n";
        std::string action;
        std::string button;
        int actionRead = 0;
        while(!file.eof()){
            char a = file.get();
            if(a == ' ' || a == '\r')
                continue;
            else if(a == '=')
                actionRead = 1;
            else if(actionRead == 0)
                action += a;
            else if(actionRead == 1 && (a == '\n' || file.eof())){
                data.actions_Buttons[action] = button;
                actionRead = 0;
                action.clear();
                button.clear();
            }
            else if(actionRead == 1)
                button += a;
        }
        file.close();
    }
    else{
        std::cerr << "File " << temp << " not found!\n";
        return -1;
    }
    return 0;
}
