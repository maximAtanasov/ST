/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#ifndef KEY_DEFINITIONS_HPP
#define KEY_DEFINITIONS_HPP

#include <cstdint>
#include <SDL_scancode.h>

//This is where the key to toggle the console is defined
#define CONSOLE_TOGGLE_KEY '`'

namespace ST {

    ///An enum containing all mouse/keyboard keys supported by the engine.
    //SDL_SCANCODE values are used for almost all keys. This should be portable, but whether that's true is unknown.
    enum class key : uint8_t {
        UNKNOWN = SDL_SCANCODE_UNKNOWN,

        //1 - 3 is unused in SDL_scancodes.h so we can use it to store mouse inputs
        MOUSELEFT = 1,
        MOUSEMIDDLE = 2,
        MOUSERIGHT = 3,

        LEFT = SDL_SCANCODE_LEFT,
        RIGHT = SDL_SCANCODE_RIGHT,
        UP = SDL_SCANCODE_UP,
        DOWN = SDL_SCANCODE_DOWN,
        A = SDL_SCANCODE_A,
        B = SDL_SCANCODE_B,
        C = SDL_SCANCODE_C,
        D = SDL_SCANCODE_D,
        E = SDL_SCANCODE_E,
        F = SDL_SCANCODE_F,
        G = SDL_SCANCODE_G,
        H = SDL_SCANCODE_H,
        I = SDL_SCANCODE_I,
        J = SDL_SCANCODE_J,
        K = SDL_SCANCODE_K,
        L = SDL_SCANCODE_L,
        M = SDL_SCANCODE_M,
        N = SDL_SCANCODE_N,
        O = SDL_SCANCODE_O,
        P = SDL_SCANCODE_P,
        Q = SDL_SCANCODE_Q,
        R = SDL_SCANCODE_R,
        S = SDL_SCANCODE_S,
        T = SDL_SCANCODE_T,
        V = SDL_SCANCODE_V,
        U = SDL_SCANCODE_U,
        W = SDL_SCANCODE_W,
        X = SDL_SCANCODE_X,
        Y = SDL_SCANCODE_Y,
        Z = SDL_SCANCODE_Z,
        ONE = SDL_SCANCODE_1,
        TWO = SDL_SCANCODE_2,
        THREE = SDL_SCANCODE_3,
        FOUR = SDL_SCANCODE_4,
        FIVE = SDL_SCANCODE_5,
        SIX = SDL_SCANCODE_6,
        SEVEN = SDL_SCANCODE_7,
        EIGHT = SDL_SCANCODE_8,
        NINE = SDL_SCANCODE_9,
        ZERO = SDL_SCANCODE_0,
        ESCAPE = SDL_SCANCODE_ESCAPE,
        ENTER = SDL_SCANCODE_RETURN,
        SPACEBAR = SDL_SCANCODE_SPACE,
        TILDE = SDL_SCANCODE_GRAVE,
        LSHIFT = SDL_SCANCODE_LSHIFT,
        BACKSPACE = SDL_SCANCODE_BACKSPACE,
        BACKSLASH = SDL_SCANCODE_BACKSLASH,
        CAPSLOCK = SDL_SCANCODE_CAPSLOCK,
        COMMA = SDL_SCANCODE_COMMA,
        EQUALS = SDL_SCANCODE_EQUALS,
        LALT = SDL_SCANCODE_LALT,
        LCTRL = SDL_SCANCODE_LCTRL,
        LBRACKET = SDL_SCANCODE_LEFTBRACKET,
        RBRACKET = SDL_SCANCODE_RIGHTBRACKET,
        MINUS = SDL_SCANCODE_MINUS,
        RALT = SDL_SCANCODE_RALT,
        RCTRL = SDL_SCANCODE_RCTRL,
        SEMICOLON = SDL_SCANCODE_SEMICOLON,
        SLASH = SDL_SCANCODE_SLASH,
        TAB = SDL_SCANCODE_TAB,
        DELETE = SDL_SCANCODE_DELETE,

        //Range from 153 to 166 in SDL_scancode.h is irrelevant to us, so we can use it to store these vars
        CONTROLLER_BUTTON_A = 153,
        CONTROLLER_BUTTON_B = 154,
        CONTROLLER_BUTTON_X = 155,
        CONTROLLER_BUTTON_Y = 156,
        CONTROLLER_BUTTON_SELECT = 157,
        CONTROLLER_BUTTON_START = 158,
        CONTROLLER_BUTTON_LEFTSTICK = 159,
        CONTROLLER_BUTTON_RIGHTSTICK = 160,
        CONTROLLER_BUTTON_LEFTSHOULDER = 161,
        CONTROLLER_BUTTON_RIGHTSHOULDER = 162,
        CONTROLLER_BUTTON_DPAD_UP = 163,
        CONTROLLER_BUTTON_DPAD_DOWN = 164,
        CONTROLLER_BUTTON_DPAD_LEFT = 165,
        CONTROLLER_BUTTON_DPAD_RIGHT = 166,
    };
}

#endif
