#include <game_manager/level/text.hpp>

text::text(Uint64 id, int x, int y, SDL_Color color, const std::string& text_string, const std::string& font, int font_size) {
    this->ID = id;
    this->x = x;
    this->y = y;
    this->color = color;
    this->text_string = text_string;
    this->font = font;
    this->font_size = font_size;
}

void text::set_visible(bool arg) {
    this->visible = arg;
}

bool text::is_visible() {
    return this->visible;
}

void text::set_x(int arg) {
    this->x = arg;
}

void text::set_y(int arg){
    this->y = arg;
}

void text::set_color(SDL_Color arg) {
    this->color = arg;
}

void text::set_font(const std::string& font) {
    this->font = font;
}

void text::set_text_string(const std::string &text) {
    this->text_string = text;
}

void text::set_font_size(int size) {
    this->font_size = size;
}

int text::get_x(){
    return x;
}

int text::get_y(){
    return y;
}

SDL_Color text::get_color(){
    return color;
}

std::string text::get_text_string(){
    return text_string;
}

std::string text::get_font (){
    return font;
}

int text::get_font_size (){
    return font_size;
}