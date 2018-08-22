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

        text(uint64_t id, int32_t x, int32_t y, SDL_Color color, const std::string &text_string, const std::string &font,
             uint8_t font_size);

        uint64_t ID;
        int32_t x, y;
        SDL_Color color;
        uint8_t font_size;
        bool is_visible = true;
        std::string text_string;
        std::string font;
    };
}

#endif