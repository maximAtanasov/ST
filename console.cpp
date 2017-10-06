#include "console.hpp"
#include "renderer.hpp"
#include "input_manager.hpp"

console::console(void){
    //empty constructor
}

int console::initialize(){
    window = {0,0, 1920, 1080/2};
    color = {255, 0, 255, 100};
    color_text = {255, 255, 255, 255};
    shown = false;
    pos = 1080/2;
    font_size = 40;
    scrollOffset = 0;
    return 0;
}

void console::draw(){
    gRenderer.draw_rectangle_filled(0, 0, 1920, 1080/2, color);
    for(auto i = entries.rbegin(); i != entries.rend(); ++i){
        if(pos-font_size - 15 + scrollOffset <= 1080/2 - font_size)
            gRenderer.draw_text("fonts/OpenSans-Regular.ttf", *i, 0, pos-font_size - 20 + scrollOffset, color_text, font_size, D_RENDERTEXT_FAST);
            pos -= font_size + 5;
    }
    pos = 1080/2;
}

void console::scroll(float scrollY){
    scrollOffset = scrollY*20;
}

void console::update(){
    scroll(gInput_manager.mouse_scroll());
    if(is_open())
        this->draw();
}

void console::write(std::string arg){
    entries.push_back(arg);
}

bool console::is_open(){
    return shown;
}

void console::close(){
    scrollOffset = 0;
    SDL_ShowCursor(SDL_DISABLE);
    shown = false;
}

void console::open(){
    SDL_ShowCursor(SDL_ENABLE);
    shown = true;
}
