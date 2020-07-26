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

//This is where the key to toggle the console is defined
#define CONSOLE_TOGGLE_KEY '`'

namespace ST {

    ///An enum containing all mouse/keyboard keys supported by the engine.
    enum class key : uint8_t {
        LEFT = 1,
        RIGHT = 2,
        UP = 3,
        DOWN = 4,
        A = 5,
        B = 6,
        C = 7,
        D = 8,
        E = 9,
        F = 10,
        G = 11,
        H = 12,
        I = 13,
        J = 14,
        K = 15,
        L = 16,
        M = 17,
        N = 18,
        O = 19,
        P = 20,
        Q = 21,
        R = 22,
        S = 23,
        T = 24,
        V = 26,
        U = 25,
        W = 27,
        X = 28,
        Y = 29,
        Z = 30,
        ONE = 31,
        TWO = 32,
        THREE = 33,
        FOUR = 34,
        FIVE = 35,
        SIX = 36,
        SEVEN = 37,
        EIGHT = 38,
        NINE = 39,
        ZERO = 40,
        ESCAPE = 41,
        ENTER = 42,
        SPACEBAR = 43,
        TILDE = 44,
        LSHIFT = 45,
        BACKSPACE = 46,
        BACKSLASH = 47,
        CAPSLOCK = 48,
        COMMA = 49,
        EQUALS = 50,
        LALT = 51,
        LCTRL = 52,
        LBRACKET = 53,
        RBRACKET = 54,
        MINUS = 55,
        RALT = 56,
        RCTRL = 57,
        SEMICOLON = 58,
        SLASH = 59,
        TAB = 60,
        MOUSELEFT = 61,
        MOUSEMIDDLE = 62,
        MOUSERIGHT = 63,
        UNKNOWN = 64,
        CONTROLLER_BUTTON_A = 65,
        CONTROLLER_BUTTON_B = 66,
        CONTROLLER_BUTTON_X = 67,
        CONTROLLER_BUTTON_Y = 68,
        CONTROLLER_BUTTON_SELECT = 69,
        CONTROLLER_BUTTON_START = 70,
        CONTROLLER_BUTTON_LEFTSTICK = 71,
        CONTROLLER_BUTTON_RIGHTSTICK = 72,
        CONTROLLER_BUTTON_LEFTSHOULDER = 73,
        CONTROLLER_BUTTON_RIGHTSHOULDER = 74,
        CONTROLLER_BUTTON_DPAD_UP = 75,
        CONTROLLER_BUTTON_DPAD_DOWN = 76,
        CONTROLLER_BUTTON_DPAD_LEFT = 77,
        CONTROLLER_BUTTON_DPAD_RIGHT = 78,
		DELETE = 79
    };
}

#endif
