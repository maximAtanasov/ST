#include <defs.hpp>

#ifndef TEXT_OBJECT_DEF
#define TEXT_OBJECT_DEF

namespace ST {

    class text {

    private:
        Uint64 ID;
        int x, y;
        SDL_Color color;
        std::string text_string;
        std::string font;
        int font_size;
        bool visible = true;

    public:
        text(Uint64 id, int x, int y, SDL_Color color, const std::string &text_string, const std::string &font,
             int font_size);

        //setters
        void set_x(int arg);

        void set_y(int arg);

        void set_color(SDL_Color arg);

        void set_text_string(const std::string &text);

        void set_font(const std::string &font);

        void set_font_size(int size);

        void set_visible(bool arg);

        //getters
        int get_x();

        int get_y();

        SDL_Color get_color();

        std::string get_text_string();

        std::string get_font();

        int get_font_size();

        bool is_visible();

    };

}

//INLINED METHODS

inline void ST::text::set_visible(bool arg) {
    this->visible = arg;
}

inline bool ST::text::is_visible() {
    return this->visible;
}

inline void ST::text::set_x(int arg) {
    this->x = arg;
}

inline void ST::text::set_y(int arg){
    this->y = arg;
}

inline void ST::text::set_color(SDL_Color arg) {
    this->color = arg;
}

inline void ST::text::set_font(const std::string& font) {
    this->font = font;
}

inline void ST::text::set_text_string(const std::string &text) {
    this->text_string = text;
}

inline void ST::text::set_font_size(int size) {
    this->font_size = size;
}

inline int ST::text::get_x(){
    return x;
}

inline int ST::text::get_y(){
    return y;
}

inline SDL_Color ST::text::get_color(){
    return color;
}

inline std::string ST::text::get_text_string(){
    return text_string;
}

inline std::string ST::text::get_font (){
    return font;
}

inline int ST::text::get_font_size (){
    return font_size;
}

#endif