#include "display_manager.hpp"

display_manager::display_manager(){
    //empty constructor
}

void display_manager::close(void){
    SDL_DestroyWindow(window);
    window = NULL;
    SDL_Quit();
}

int display_manager::initialize(void){
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << "\n";
        exit(1);
    }
	SDL_GetCurrentDisplayMode(0, &DM);
	width = DM.w;
    height = DM.h;
    window = SDL_CreateWindow
    (
        "MEngine", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
		    width,
        height,
        SDL_WINDOW_RESIZABLE
    );
    std::cout << "Current screen resolution is " << width << "x" << height << "\n";
    //Load icon
    SDL_Surface* icon = IMG_Load("levels/icon.png");
    SDL_SetWindowIcon(window, icon);
    return 0;
}

SDL_Window* display_manager::get_window(void){
    return window;
}

void display_manager::set_fullscreen(void){
    SDL_SetWindowFullscreen(window, 1);
}

void display_manager::set_windowed(void){
    SDL_SetWindowFullscreen(window, 0);
}
