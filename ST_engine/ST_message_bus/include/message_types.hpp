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
    SET_DARKNESS,
    PLAY_SOUND,
    PLAY_MUSIC,
    STOP_MUSIC,
    SET_AUDIO_ENABLED,
    AUDIO_ENABLED,
    PAUSE_MUSIC,
    SET_SOUNDS_VOLUME,
    SOUNDS_VOLUME_LEVEL,
    SET_MUSIC_VOLUME,
    MUSIC_VOLUME_LEVEL,
    LOAD_LEVEL,
    START_LEVEL,
    UNLOAD_LEVEL,
    LOG_ERROR,
    LOG_SUCCESS,
    LOG_INFO,
    CONSOLE_TOGGLE,
    CONSOLE_CLEAR,
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
    END_GAME,
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
    SET_WINDOW_BRIGHTNESS,
    EXECUTE_SCRIPT,
    SET_FULLSCREEN,
    FULLSCREEN_STATUS,
    LOAD_ASSET,
    UNLOAD_ASSET,
    LOAD_BINARY,
    SET_INTERNAL_RESOLUTION,
    SET_WINDOW_RESOLUTION,
    SET_VSYNC,
    VSYNC_STATE,
    LEFT_TRIGGER,
    RIGHT_TRIGGER,
    RIGHT_STICK_VERTICAL,
    RIGHT_STICK_HORIZONTAL,
    LEFT_STICK_VERTICAL,
    LEFT_STICK_HORIZONTAL
};

#endif //SLAVIC_TALES_MESSAGE_TYPES_HPP
