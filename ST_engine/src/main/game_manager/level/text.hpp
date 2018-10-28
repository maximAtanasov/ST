/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <defs.hpp>

#ifndef TEXT_OBJECT_DEF
#define TEXT_OBJECT_DEF

namespace ST {

    ///This struct represents text objects in the game
    /**
     * It is used for any user-defined or game-specific text. (So not the fps counter, console text, etc.).
     */
    struct text {

        text(int32_t x, int32_t y, SDL_Color color, const std::string &text_string, const std::string &font,
             uint8_t font_size);

        std::string text_string;
        std::string font;
        int32_t x, y;
        SDL_Color color;
        bool is_visible = true;
        uint8_t font_size;
    };
//    static_assert(sizeof(ST::text) == 80, "class 'text' is not sized properly, maybe you have misaligned the fields");
}

#endif