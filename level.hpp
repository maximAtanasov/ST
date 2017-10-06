#ifndef LVL_DEF
#define LVL_DEF

#include "defs.hpp"
#include "renderer.hpp"
#include "audio_manager.hpp"
#include "input_manager.hpp"
#include "entity.hpp"

typedef struct level_data{
    std::unordered_map<std::string, std::string> actions_Buttons;
    std::vector<entity*> entities;
    int background;
} level_data;

class level{
    private:
        std::string name;
        level_data data;
        int load_input_conf(void);
    public:
        level(std::string);
        void load(void);
        void unload(void);
        inline level_data* get_data(void){
            return &data;
        }
        std::string get_name(void);
};

#endif
