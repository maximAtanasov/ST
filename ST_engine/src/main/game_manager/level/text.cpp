/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <game_manager/level/text.hpp>

/**
 * Constructor for the text objects.
 * @param id The id for the object.
 * @param x The X position.
 * @param y The Y position.
 * @param color The color of the text (RGBA).
 * @param text_string The text itself.
 * @param font The font to use (Must be already loaded).
 * @param font_size The size of the font.
 */
ST::text::text(uint64_t id, int x, int y, SDL_Color color, const std::string& text_string, const std::string& font, uint8_t font_size) {
    this->ID = id;
    this->x = x;
    this->y = y;
    this->color = color;
    this->text_string = text_string;
    this->font = font;
    this->font_size = font_size;
}