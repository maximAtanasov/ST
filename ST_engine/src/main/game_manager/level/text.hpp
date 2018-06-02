/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <defs.hpp>

#ifndef TEXT_OBJECT_DEF
#define TEXT_OBJECT_DEF

namespace ST {

    ///This class represents text objects in the game
    /**
     * It is used for any user-defined or game-specific text. (So not the fps counter, console text, etc.).
     */
    class text {

    private:
        uint64_t ID;
        int32_t x, y;
        SDL_Color color;
        std::string text_string;
        std::string font;
        uint8_t font_size;
        bool visible = true;

    public:
        text(uint64_t id, int32_t x, int32_t y, SDL_Color color, const std::string &text_string, const std::string &font,
             uint8_t font_size);

        //setters
        void set_x(int32_t arg);

        void set_y(int32_t arg);

        void set_color(SDL_Color arg);

        void set_text_string(const std::string &text);

        void set_font(const std::string &font);

        void set_font_size(uint8_t size);

        void set_visible(bool arg);

        //getters
        int32_t get_x();

        int32_t get_y();

        SDL_Color get_color();

        std::string get_text_string();

        std::string get_font();

        uint8_t get_font_size();

        bool is_visible();

    };

}

//INLINED METHODS

/**
 * Set the text object to visible or not visible.
 * @param arg True - visible, False - not visible.
 */
inline void ST::text::set_visible(bool arg) {
    this->visible = arg;
}

/**
 * Tells if the text object is visible.
 * @return True - visible, false - not visible.
 */
inline bool ST::text::is_visible() {
    return this->visible;
}

/**
 * Set the X position of the text object.
 * @param arg the X coordinate.
 */
inline void ST::text::set_x(int32_t arg) {
    this->x = arg;
}

/**
 * Set the Y position of the text object.
 * @param arg the Y coordinate.
 */
inline void ST::text::set_y(int32_t arg){
    this->y = arg;
}

/**
 * Set the color of the text.
 * @param arg SDL_Color struct (RGBA).
 */
inline void ST::text::set_color(SDL_Color arg) {
    this->color = arg;
}

/**
 * Set the font for the text.
 * @param font A string with the font name. (Font must already be loaded).
 */
inline void ST::text::set_font(const std::string& font) {
    this->font = font;
}

/**
 * Set the text itself.
 * @param text A string containing the text, this can be UTF-8 as well.
 */
inline void ST::text::set_text_string(const std::string &text) {
    this->text_string = text;
}

/**
 * Set the font size for the text. (Font must be loaded with that font size).
 * @param size The size of the font.
 */
inline void ST::text::set_font_size(uint8_t size) {
    this->font_size = size;
}

/**
 * Get the X position of the text object.
 * @return the X coordinate.
 */
inline int32_t ST::text::get_x(){
    return x;
}

/**
 * Get the Y position of the text object.
 * @return the Y coordinate.
 */
inline int32_t ST::text::get_y(){
    return y;
}

/**
 * Get the color of the font.
 * @return SDL_Color struct (RGBA).
 */
inline SDL_Color ST::text::get_color(){
    return color;
}

/**
 * Return the text itself.
 * @return a std::string containing the text.
 */
inline std::string ST::text::get_text_string(){
    return text_string;
}

/**
 * Get the name of the font.
 * @return The name of fthe font.
 */
inline std::string ST::text::get_font (){
    return font;
}

/**
 * Get the size of the font.
 * @return Font size.
 */
inline uint8_t ST::text::get_font_size (){
    return font_size;
}

#endif