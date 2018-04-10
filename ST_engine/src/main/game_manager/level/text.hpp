#include <defs.hpp>

#ifndef TEXT_OBJECT_DEF
#define TEXT_OBJECT_DEF

class text{

private:
    Uint64 ID;
    int x, y;
    SDL_Color color;
    std::string text_string;
    std::string font;
    int font_size;
    bool visible = true;

public:
    text(Uint64 id, int x, int y, SDL_Color color, const std::string& text_string, const std::string& font, int font_size);

    //setters
    void set_x(int arg);
    void set_y(int arg);
    void set_color(SDL_Color arg);
    void set_text_string(const std::string &text);
    void set_font(const std::string& font);
    void set_font_size(int size);
    void set_visible(bool arg);

    //getters
    int get_x();
    int get_y();
    SDL_Color get_color();
    std::string get_text_string();
    std::string get_font ();
    int get_font_size();
    bool is_visible();

};

#endif