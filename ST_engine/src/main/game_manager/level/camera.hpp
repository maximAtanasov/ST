/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#ifndef CAMERA_DEF
#define CAMERA_DEF

namespace ST {

    ///The camera object - represents the viewport for the game.
    struct camera {
        int x;
        int y;
        int limitX1;
        int limitX2;
        int limitY1;
        int limitY2;
    };
}

#endif
