/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 *
 */

#ifndef ST_TEXTURE_HPP
#define ST_TEXTURE_HPP

#include "SDL_render.h"

namespace ST::renderer_sdl {

    class texture {
        public:
            uint16_t height;
            uint16_t width;
            uint16_t atlas_h_offset;
            uint16_t atlas_v_offset;
            SDL_Texture* atlas;
    };
}

#endif //ST_TEXTURE_HPP
