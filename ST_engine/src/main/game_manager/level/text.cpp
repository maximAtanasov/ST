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