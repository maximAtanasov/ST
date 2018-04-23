/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
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
