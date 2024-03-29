/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#ifndef ST_MESSAGE_TYPES_HPP
#define ST_MESSAGE_TYPES_HPP

//all the different messages are defined in this enum
//to define a new one just add its name here - they are all used as integers

enum msg_type : uint8_t {

    /**
     * base_data0 must contain an 8 bit value describing the darkness level. (uint8_t)
     */
    SET_DARKNESS,

    /**
     * base_data0 must contain a 32 bit integer.
     * The first 16 bits must describe the sample to be played. (uint16_t)
     * The following 8 must describe the volume. (uint8_t)
     * The last 8 must describe the amount of loops ot play. (int8_t)
     */
    PLAY_SOUND,

    /**
     * base_data0 must contain a 32 bit integer.
     * The first 16 bits must describe the music to be played. (uint16_t)
     * The following 8 must describe the volume. (uint8_t)
     * The last 8 must describe the amount of loops ot play. (int8_t)
     */
    PLAY_MUSIC,

    /**
     * Must contain no data.
     */
    STOP_MUSIC,

    /**
     * base_data0 must be set and interpreted as a boolean value.
     */
    SET_AUDIO_ENABLED,

    /**
     * base_data0 must be set and interpreted as a boolean value.
     */
    AUDIO_ENABLED,

    /**
     * Must contain no data.
     */
    PAUSE_MUSIC,

    /**
     * base_data0 must contain an 8 bit value describing the volume level. (uint8_t)
     */
    SET_SOUNDS_VOLUME,

    /**
     * base_data0 must contain an 8 bit value describing the volume level. (uint8_t)
     */
    SOUNDS_VOLUME_LEVEL,

    /**
     * base_data0 must contain an 8 bit value describing the volume level. (uint8_t)
     */
    SET_MUSIC_VOLUME,

    /**
     * base_data0 must contain an 8 bit value describing the volume level. (uint8_t)
     */
    MUSIC_VOLUME_LEVEL,

    /**
     * data must contain a std::string created with make_data()
     */
    LOAD_LEVEL,

    /**
     * data must contain a std::string created with make_data()
     */
    START_LEVEL,

    /**
     * data must contain a std::string created with make_data()
     */
    UNLOAD_LEVEL,

    /**
     * data must contain a std::string created with make_data()
     */
    RELOAD_LEVEL,

    /**
     * data must contain a std::string created with make_data()
     */
    LOG_ERROR,

    /**
     * data must contain a std::string created with make_data()
     */
    LOG_SUCCESS,

    /**
     * data must contain a std::string created with make_data()
     */
    LOG_INFO,

    /**
     * Must contain no data.
     */
    CONSOLE_TOGGLE,

    /**
     * Must contain no data.
     */
    CONSOLE_CLEAR,

    /**
     * base_data0 must be set and interpreted as a boolean value.
     */
    SHOW_COLLISIONS,

    /**
     * data must contain a std::string* created with make_data()
     */
    LOAD_LIST,

    /**
     * data must contain a std::string* created with make_data()
     */
    UNLOAD_LIST,

    /**
     * data must contain a ska::bytell_hash_map<uint16_t, SDL_Surface *>** created with make_data()
     */
    SURFACES_ASSETS,

    /**
     * data must contain a ska::bytell_hash_map<uint16_t, TTF_Font *>** created with make_data()
     */
    FONTS_ASSETS,

    /**
     * data must contain a ska::bytell_hash_map<uint16_t, Mix_Chunk *>** created with make_data()
     */
    CHUNKS_ASSETS,

    /**
     * data must contain a ska::bytell_hash_map<uint16_t, Mix_Music *>** created with make_data()
     */
    MUSIC_ASSETS,

    /**
     * base_data0 must be set and interpreted as type ST::key
     */
    KEY_PRESSED,

    /**
     * base_data0 must be set and interpreted as type ST::key
     */
    KEY_HELD,

    /**
     * base_data0 must be set and interpreted as type ST::key
     */
    KEY_RELEASED,

    /**
     * base_data0 must be set and interpreted as a int32_t value.
     */
    MOUSE_X,

    /**
     * base_data0 must be set and interpreted as a int32_t value.
     */
    MOUSE_Y,

    /**
     * base_data0 must be set and interpreted as a int16_t value.
     */
    MOUSE_SCROLL,


    /**
     * base_data0 must be set and interpreted as a int8_t value.
     */
    SET_GRAVITY,

    /**
     * base_data0 must be set and interpreted as a int8_t value.
     */
    SET_FRICTION,

    /**
     * base_data0 must be set and interpreted as a int32_t value.
     */
    SET_FLOOR,

    /**
     * Must contain no data.
     */
    END_GAME,

    /**
     * Must contain no data.
     */
    STOP_ALL_SOUNDS,


    /**
     * base_data0 must be set. The first 16 bits must describe the width.
     * The last 16 bits must describe the height.
     */
    VIRTUAL_SCREEN_COORDINATES,

    /**
     * base_data0 must be set. The first 16 bits must describe the width.
     * The last 16 bits must describe the height.
     */
    REAL_SCREEN_COORDINATES,

    /**
     * Must contain no data.
     */
    PAUSE_PHYSICS,

    /**
     * Must contain no data.
     */
    UNPAUSE_PHYSICS,

    /**
     * Must contain no data.
     */
    START_TEXT_INPUT,

    /**
     * Must contain no data.
     */
    STOP_TEXT_INPUT,

    /**
     * data must contain a std::string created with make_data()
     */
    TEXT_STREAM,

    /**
     * Must contain no data.
     */
    CLEAR_TEXT_STREAM,

    /**
     * base_data0 must be set and interpreted as a boolean value.
     */
    SHOW_MOUSE,

    /**
     * base_data0 must be set and interpreted as a boolean value.
     */
    SHOW_FPS,

    /**
     * base_data0 must be set and interpreted as a boolean value.
     */
    SHOW_METRICS,

    /**
     * base_data0 must be set and interpreted as a boolean value.
     */
    ENABLE_LIGHTING,

    /**
     * base_data0 must be set and interpreted as type ST::key
     */
    REGISTER_KEY,

    /**
     * base_data0 must be set and interpreted as type ST::key
     */
    UNREGISTER_KEY,
    SET_WINDOW_BRIGHTNESS,

    /**
     * data must contain a std::string created with make_data()
     */
    EXECUTE_SCRIPT,

    /**
     * base_data0 must be set and interpreted as a boolean value.
     */
    SET_FULLSCREEN,

    /**
     * base_data0 must be set and interpreted as a boolean value.
     */
    FULLSCREEN_STATUS,

    /**
     * data must contain a std::string created with make_data()
     */
    LOAD_ASSET,

    /**
     * data must contain a std::string created with make_data()
     */
    UNLOAD_ASSET,

    /**
     * data must contain a std::string created with make_data()
     */
    LOAD_BINARY,

    /**
     * base_data0 must be set. The first 16 bits must describe the width.
     * The last 16 bits must describe the height.
     */
    SET_INTERNAL_RESOLUTION,

    /**
     * base_data0 must be set. The first 16 bits must describe the width.
     * The last 16 bits must describe the height.
     */
    SET_WINDOW_RESOLUTION,


    /**
     * base_data0 must be set and interpreted as a boolean value.
     */
    SET_VSYNC,

    /**
     * base_data0 must be set and interpreted as a boolean value.
     */
    VSYNC_STATE,

    /**
     * base_data0 must be set and interpreted as a int16_t value.
     */
    LEFT_TRIGGER,

    /**
     * base_data0 must be set and interpreted as a int16_t value.
     */
    RIGHT_TRIGGER,

    /**
     * base_data0 must be set and interpreted as a int16_t value.
     */
    RIGHT_STICK_VERTICAL,

    /**
     * base_data0 must be set and interpreted as a int16_t value.
     */
    RIGHT_STICK_HORIZONTAL,

    /**
     * base_data0 must be set and interpreted as a int16_t value.
     */
    LEFT_STICK_VERTICAL,

    /**
     * base_data0 must be set and interpreted as a int16_t value.
     */
    LEFT_STICK_HORIZONTAL,

    /**
     * data must contain a std::tuple<float, uint32_t>* created with make_data.
     */
    CONTROLLER_RUMBLE,

    /**
     * base_data0 must be set and interpreted as a int16_t value.
     */
    SET_LEFT_JOYSTICK_HORIZONTAL_THRESHOLD,

    /**
     * base_data0 must be set and interpreted as a int16_t value.
     */
    SET_LEFT_JOYSTICK_VERTICAL_THRESHOLD,

    /**
     * base_data0 must be set and interpreted as a int16_t value.
     */
    SET_RIGHT_JOYSTICK_HORIZONTAL_THRESHOLD,

    /**
     * base_data0 must be set and interpreted as a int16_t value.
     */
    SET_RIGHT_JOYSTICK_VERTICAL_THRESHOLD,

    /**
     * base_data0 must be set and interpreted as a int16_t value.
     */
    SET_RIGHT_TRIGGER_THRESHOLD,

    /**
     * base_data0 must be set and interpreted as a int16_t value.
     */
    SET_LEFT_TRIGGER_THRESHOLD
};

#endif //ST_MESSAGE_TYPES_HPP
