#include "game_manager.hpp"

//initializes the game_manager and loads "main"
int game_manager::initialize(message_bus* msg_bus, task_manager* tsk_mngr){

    gScript_backend = new lua_backend();
    gScript_backend->initialize(gMessage_bus, this);
    gMessage_bus = msg_bus;
    gTask_manager = tsk_mngr;

    //create a subscriber and subscribe to messages
    msg_sub = new subscriber();
    gMessage_bus->subscribe(LOAD_LEVEL, msg_sub);
    gMessage_bus->subscribe(START_LEVEL, msg_sub);
    gMessage_bus->subscribe(UNLOAD_LEVEL, msg_sub);
    gMessage_bus->subscribe(KEY_PRESSED, msg_sub);
    gMessage_bus->subscribe(KEY_HELD, msg_sub);
    gMessage_bus->subscribe(KEY_RELEASED, msg_sub);
    gMessage_bus->subscribe(MOUSE_X, msg_sub);
    gMessage_bus->subscribe(MOUSE_Y, msg_sub);
    gMessage_bus->subscribe(VSYNC_IS_ON, msg_sub);
    gMessage_bus->subscribe(VSYNC_IS_OFF, msg_sub);
    gMessage_bus->subscribe(END_GAME, msg_sub);
    gMessage_bus->subscribe(VOLUME_LEVEL, msg_sub);
    gMessage_bus->subscribe(SHOW_MOUSE, msg_sub);
    gMessage_bus->subscribe(EXECUTE_SCRIPT, msg_sub);
    gMessage_bus->subscribe(FULLSCREEN_STATUS, msg_sub);
    //initialize initial states of keys
    reset_keys();

    vsync_flag = true;
    fullscreen_status = false;
    volume_level = 100;

    //Load the first level
    load_level("main");
    start_level("main");
    SDL_AtomicSet(&end_game, 1);
    return 0;
}

bool game_manager::key_pressed(size_t arg){
    return keys_pressed_data[(Uint8)(get_level_data()->actions_Buttons[arg])];
}

bool game_manager::key_held(size_t arg){
    return keys_held_data[(Uint8)get_level_data()->actions_Buttons[arg]];
}

bool game_manager::key_released(size_t arg){
    return keys_released_data[(Uint8)get_level_data()->actions_Buttons[arg]];
}

void game_manager::reset_keys(){
    for(int i = 0; i < 64; i++){
        keys_pressed_data[i] = false;
        keys_held_data[i] = false;
        keys_released_data[i] = false;
    }
}

void game_manager::update(){
    handle_messages();
    run_level_loop();
}

void game_manager::handle_messages(){
    message* temp = msg_sub->get_next_message();
    while(temp != nullptr){
        if(temp->msg_name == LOAD_LEVEL){
            auto name = (std::string*)temp->get_data();
            load_level(*name);
        }
        else if(temp->msg_name == START_LEVEL){
            auto name = (std::string*)temp->get_data();
            start_level(*name);
        }
        else if(temp->msg_name == UNLOAD_LEVEL){
            auto name = (std::string*)temp->get_data();
            unload_level(*name);
        }
        else if(temp->msg_name == KEY_PRESSED){
            Uint8 key_index = *(Uint8*)temp->get_data();
            keys_pressed_data[key_index] = true;
            keys_held_data[key_index] = false;
            keys_released_data[key_index] = false;
        }
        else if(temp->msg_name == KEY_HELD){
            Uint8 key_index = *(Uint8*)temp->get_data();
            keys_pressed_data[key_index] = false;
            keys_held_data[key_index] = true;
            keys_released_data[key_index] = false;
        }
        else if(temp->msg_name == KEY_RELEASED){
            Uint8 key_index = *(Uint8*)temp->get_data();
            keys_pressed_data[key_index] = false;
            keys_held_data[key_index] = false;
            keys_released_data[key_index] = true;
        }
        else if(temp->msg_name == MOUSE_X){
            int x = *(int*)temp->get_data();
            mouse_x = x;
        }
        else if(temp->msg_name == MOUSE_Y){
            int y = *(int*)temp->get_data();
            mouse_y = y;
        }
        else if(temp->msg_name == VOLUME_LEVEL){
            int val = *(int*)temp->get_data();
            volume_level = val;
        }
        else if(temp->msg_name == VSYNC_IS_ON){
            vsync_flag = true;
        }
        else if(temp->msg_name == VSYNC_IS_OFF){
            vsync_flag = false;
        }
        else if(temp->msg_name == END_GAME){
            SDL_AtomicSet(&end_game, 0);
        }
        else if(temp->msg_name == SHOW_MOUSE){
            auto arg = (bool*)temp->get_data();
            if(*arg){
                SDL_ShowCursor(SDL_ENABLE);
            }else if(!*arg){
                SDL_ShowCursor(SDL_DISABLE);
            }
        }
        else if(temp->msg_name == EXECUTE_SCRIPT){
            auto scipt = (std::string*)temp->get_data();
            gScript_backend->run_script(*scipt);
        }
        else if(temp->msg_name == FULLSCREEN_STATUS){
            bool arg = *(bool*)temp->get_data();
            fullscreen_status = arg;
        }
        destroy_msg(temp);
        temp = msg_sub->get_next_message();
    }
}

bool game_manager::game_is_running(){
    return (bool)SDL_AtomicGet(&end_game);
}

//methods related to input

int game_manager::get_mouse_x(){
    return mouse_x;
}

int game_manager::get_mouse_y(){
    return mouse_y;
}

//methods related to loading levels
void game_manager::load_level(const std::string& level_name){
    for(auto& i: levels) {
        if (i->get_name() == level_name) {
            return;
        }
    }
    auto temp = new level(level_name, gMessage_bus);
    temp->load();
    levels.push_back(temp);

    //current level pointer must be reset, because apparently adding to the vector changes the pointer address
    //(makes sense as it has to reallocate the whole thing)
    for(auto& i: levels) {
        if (i->get_name() == active_level) {
            current_level_pointer = i;
            break;
        }
    }
}

void game_manager::unload_level(const std::string& level_name){
    for(Uint32 i = 0; i < levels.size(); i++)
        if(levels[i]->get_name() == level_name){
            levels[i]->unload();
            levels.erase(levels.begin()+i);
            break;
        }
}

void game_manager::start_level(const std::string& level_name){
    gScript_backend->close();
    gScript_backend->initialize(gMessage_bus, this);
    active_level = level_name;
    //set current level pointer
    for(auto& i: levels) {
        if(i->get_name() == active_level){
            current_level_pointer = i;
            break;
        }
    }
    get_level_data()->lights.clear();
    get_level_data()->entities.clear();

    //construct level
    get_level_data()->Camera.x = 0;
    get_level_data()->Camera.y = 0;


    std::string temp = "levels/";
    temp = temp + active_level;
    temp = temp + "/level.lua";
    gScript_backend->run_file(temp);

    //load level loop
    temp = "levels/";
    temp = temp + active_level;
    temp = temp + "/loop.lua";
    gScript_backend->load_file(temp);
    gScript_backend->set_global("loop");

    for(auto i : get_level_data()->actions_Buttons) {
        gMessage_bus->send_msg(make_msg(REGISTER_KEY, make_data<key>(i.second)));
    }
}

std::string game_manager::get_active_level(){
    return active_level;
}

void game_manager::close(){
    handle_messages();
    delete msg_sub;
    gScript_backend->close();
}

void game_manager::run_level_loop() {
    gScript_backend->run_global("loop");
}

level_data* game_manager::get_level_data() {
    return current_level_pointer->get_data();
}

level game_manager::get_level(){
	return *current_level_pointer;
}

