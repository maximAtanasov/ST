/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#ifndef SLAVIC_TALES_MESSAGE_TYPES_HPP
#define SLAVIC_TALES_MESSAGE_TYPES_HPP

//all the different messages are defined in this enum
//to define a new one just add its name here - they are all used as integers

enum msg_type {
    VSYNC_ON,
    VSYNC_OFF,
    SET_DARKNESS,
    PLAY_SOUND,
    PLAY_MUSIC,
    STOP_MUSIC,
    TOGGLE_AUDIO,
    LOAD_LEVEL,
    START_LEVEL,
    UNLOAD_LEVEL,
    CONSOLE_WRITE,
    CONSOLE_TOGGLE,
    SHOW_COLLISIONS,
    LOAD_LIST,
    UNLOAD_LIST,
    ASSETS,
    KEY_PRESSED,
    KEY_HELD,
    KEY_RELEASED,
    MOUSE_X,
    MOUSE_Y,
    MOUSE_SCROLL,
    SET_GRAVITY,
    SET_FRICTION,
    SET_FLOOR,
    VSYNC_IS_OFF,
    VSYNC_IS_ON,
    END_GAME,
    VOLUME_LEVEL,
    STOP_ALL_SOUNDS,
    VIRTUAL_SCREEN_COORDINATES,
    REAL_SCREEN_COORDINATES,
    PAUSE_PHYSICS,
    UNPAUSE_PHYSICS,
    START_TEXT_INPUT,
    STOP_TEXT_INPUT,
    TEXT_STREAM,
    CLEAR_TEXT_STREAM,
    SHOW_MOUSE,
    SHOW_FPS,
    ENABLE_LIGHTING,
    REGISTER_KEY,
    UNREGISTER_KEY,
    SET_VOLUME,
    SET_WINDOW_BRIGHTNESS,
    EXECUTE_SCRIPT,
    SET_FULLSCREEN,
    FULLSCREEN_STATUS,
    LOAD_ASSET,
    UNLOAD_ASSET,
    LOAD_BINARY
};

#endif //SLAVIC_TALES_MESSAGE_TYPES_HPP
