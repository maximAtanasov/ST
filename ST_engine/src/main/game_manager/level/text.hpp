/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#ifndef TEXT_OBJECT_DEF
#define TEXT_OBJECT_DEF

#include <cstdint>
#include <SDL_pixels.h>
#include <string>

namespace ST {

    ///This struct represents text objects in the game
    /**
     * It is used for any user-defined or game-specific text. (So not the fps counter, console text, etc.).
     */
    struct text {

        text(int32_t x, int32_t y, SDL_Color color, const std::string &text_string, uint16_t font);

        //32 bytes
        std::string text_string;

        //8 bytes
        int32_t x, y;

        //4 bytes
        SDL_Color color{};

        //2 bytes
        uint16_t font;

        //1 byte
        bool is_visible = true;

        //1 byte padding
        uint8_t padding{};
    };
}

#endif