#include "display_manager.hpp"
#include "log.hpp"

void display_manager::close(){
    delete msg_sub;
    SDL_FreeSurface(icon);
    SDL_DestroyWindow(window);
    window = nullptr;
    SDL_Quit();
}

int display_manager::initialize(message_bus* msg_bus, task_manager* tsk_mngr){
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
    if(IMG_Init(IMG_INIT_PNG) < 0){
        fprintf(stderr, "Failed to initialize SDL_IMG: %s\n", IMG_GetError());
        exit(1);
    }
    gTask_manager = tsk_mngr;
    gMessage_bus = msg_bus;
	SDL_GetDisplayMode(0, 0, &DM);
	width = DM.w;
    height = DM.h;
    window = SDL_CreateWindow
    (
        "SlavicTales", 0,
        0,
		width,
        height,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
    );
    log(INFO, "Current screen resolution is " + std::to_string(width) + "x" + std::to_string(height));
	gMessage_bus->send_msg(make_msg(REAL_SCREEN_COORDINATES, make_data<std::tuple<int,int>>(std::make_tuple(width, height))));

    //Load and set icon
    icon = IMG_Load("levels/icon.png");
    SDL_SetWindowIcon(window, icon);

    msg_sub = new subscriber();
    gMessage_bus->subscribe(SET_FULLSCREEN, msg_sub);
    gMessage_bus->subscribe(SET_WINDOW_BRIGHTNESS, msg_sub);
    return 0;
}

void display_manager::update_task(void* mngr){
    auto self = (display_manager*)mngr;
    self->handle_messages();
}

void display_manager::update(){
    gTask_manager->start_task_lockfree(new task(update_task, this, nullptr, -1));
}

void display_manager::handle_messages(){
    message* temp = msg_sub->get_next_message();
    while(temp != nullptr){
        if(temp->msg_name == SET_FULLSCREEN){
            auto arg = static_cast<bool*>(temp->get_data());
            set_fullscreen(*arg);
            gMessage_bus->send_msg(make_msg(FULLSCREEN_STATUS, make_data<bool>(*arg)));
        }
        else if(temp->msg_name == SET_WINDOW_BRIGHTNESS){
            auto arg = static_cast<float*>(temp->get_data());
            set_brightness(*arg);
            log(SUCCESS, "Brightness set to: " + std::to_string(*arg));
        }
        destroy_msg(temp);
        temp = msg_sub->get_next_message();
    }
}

//returns a handle to the SDL_Window* (can be used for window management if this is ever to be used with pure OpenGL or Vulkan)
SDL_Window* display_manager::get_window(){
    return window;
}

void display_manager::set_fullscreen(bool arg){
    if(arg) {
        SDL_SetWindowFullscreen(window, 1);
    }else{
        SDL_SetWindowFullscreen(window, 0);
    }
}

void display_manager::set_brightness(float arg) {
    SDL_SetWindowBrightness(this->window, arg);
}
