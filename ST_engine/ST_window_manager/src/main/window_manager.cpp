/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#include <window_manager.hpp>
#include <SDL.h>
#include <SDL_image.h>

/**
 * Closes the Window Manager.
 * Destroys the window and quits SDL.
 */
window_manager::~window_manager(){
    SDL_FreeSurface(icon);
    SDL_DestroyWindow(window);
    window = nullptr;
    SDL_Quit();
}

/**
 * Initializes the window and SDL.
 * @param msg_bus a pointer to the global message bus
 * @param tsk_mngr a pointer to the global task manager
 */
window_manager::window_manager(message_bus* msg_bus, task_manager* tsk_mngr, const std::string& window_name){
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
    if(IMG_Init(IMG_INIT_PNG | IMG_INIT_WEBP) < 0){
        fprintf(stderr, "Failed to initialize SDL_IMG: %s\n", IMG_GetError());
        exit(1);
    }
    gTask_manager = tsk_mngr;
    gMessage_bus = msg_bus;
	SDL_GetDisplayMode(0, 0, &DM);
	width = static_cast<int16_t>(DM.w);
    height = static_cast<int16_t>(DM.h);
    window = SDL_CreateWindow(window_name.c_str(), 0, 0, width, height, SDL_WINDOW_OPENGL);
    gMessage_bus->send_msg(make_msg(LOG_INFO, make_data<std::string>("Current screen resolution is " + std::to_string(width) + "x" + std::to_string(height))));
    gMessage_bus->send_msg(make_msg(REAL_SCREEN_COORDINATES, make_data(std::make_tuple(width, height))));

    //Load and set icon
    icon = IMG_Load("levels/icon.png");
    SDL_SetWindowIcon(window, icon);

    gMessage_bus->subscribe(SET_FULLSCREEN, &msg_sub);
    gMessage_bus->subscribe(SET_WINDOW_RESOLUTION, &msg_sub);
}

/**
 * Performs the update for the window_manager on a task thread.
 * @param arg pointer to an window_manager (a <b>this</b> pointer basically) as the
 * function must be static.
 */
void window_manager::update_task(void* mngr){
    auto self = static_cast<window_manager*>(mngr);
    self->handle_messages();
}

/**
 * Retrieves messages from the subscriber object and
 * performs the appropriate actions.
 */
void window_manager::handle_messages(){
    message* temp = msg_sub.get_next_message();
    while(temp != nullptr){
        if(temp->msg_name == SET_FULLSCREEN){
            auto arg = static_cast<bool*>(temp->get_data());
            set_fullscreen(*arg);
            gMessage_bus->send_msg(make_msg(FULLSCREEN_STATUS, make_data<bool>(*arg)));
        }
        else if(temp->msg_name == SET_WINDOW_BRIGHTNESS){
            auto arg = static_cast<float*>(temp->get_data());
            set_brightness(*arg);
            gMessage_bus->send_msg(make_msg(LOG_SUCCESS, make_data<std::string>("Brightness set to: " + std::to_string(*arg))));
        }
        else if(temp->msg_name == SET_WINDOW_RESOLUTION){
            auto res = *static_cast<std::tuple<int16_t, int16_t>*>(temp->get_data());
            if(std::get<0>(res) != width && std::get<1>(res) != height) {
                gMessage_bus->send_msg(make_msg(REAL_SCREEN_COORDINATES, make_data(res)));
                SDL_SetWindowSize(window, std::get<0>(res), std::get<1>(res));
                SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
                width = std::get<0>(res);
                height = std::get<1>(res);
            }
        }
        destroy_msg(temp);
        temp = msg_sub.get_next_message();
    }
}

/**
 *
 * @return Returns a handle to the SDL_Window*
 */
SDL_Window* window_manager::get_window(){
    return window;
}

/**
 * Sets the window to fullscreen or windowed mode
 * @param arg True for fullscreen or false for windowed.
 */
void window_manager::set_fullscreen(bool arg){
#ifdef _MSC_VER
    if(arg && !(SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN)) {
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    }
    else if(!arg && (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN)){
	    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		SDL_SetWindowFullscreen(window, 0);
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		SDL_SetWindowFullscreen(window, 0);
		SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    }
#else
    if(arg && !(SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN)) {
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    }
    else if(!arg && (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN)){
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		SDL_SetWindowFullscreen(window, 0);
		SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    }
#endif
}

/**
 * This function should set the brightness.
 * However, it does not function.
 * @param arg a float indicating the value.
 */
void window_manager::set_brightness(float arg) {
    SDL_SetWindowBrightness(this->window, arg);
}