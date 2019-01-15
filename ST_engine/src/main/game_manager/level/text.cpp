/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#include <game_manager/level/text.hpp>

/**
 * Constructor for the text objects.
 * @param x The X position.
 * @param y The Y position.
 * @param color The color of the text (RGBA).
 * @param text_string The text itself.
 * @param font The font to use (Must be already loaded).
 * @param font_size The size of the font.
 */
ST::text::text(int32_t x, int32_t y, SDL_Color color, const std::string& text_string, const std::string& font, uint8_t font_size) {
    this->x = x;
    this->y = y;
    this->color = color;
    this->text_string = text_string;
    this->font = font;
    this->font_size = font_size;
}