/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#ifndef DEVCONSOLE_DEF
#define DEVCONSOLE_DEF

#include <defs.hpp>
#include <console/console_log.hpp>
#include <message_bus/message_bus.hpp>

///This object represents the console window.
/**
 * Displays all in-game logs and provides an interface to the lua scripting engine. <br>
 *
 * Messages this object listens to: <br>
 *
 * <b>CONSOLE_WRITE</b> - Writes a log to the console window. <br>
 *
 *  Message must contain: a pointer to a <b>ST::console_log</b> object.
 *
 * <b>CONSOLE_TOGGLE</b> - Toggles the console window. <br>
 *
 * Message must contain: a <b>nullptr</b>
 *
 * <b>MOUSE_SCROLL</b> - Scrolls the console window the given amount. <br>
 *
 * Message must contain: a pointer to an <b>int</b>.
 *
 * <b>KEY_PRESSED</b> - Checks if a key is pressed - only cares about the ENTER key. <br>
 *
 * Message must contain: a pointer to an <b>ST::key</b> object.
 *
 * <b>TEXT_STREAM</b> - Updates the internal composition given a pointer to a std::string. <br>
 *
 * Message must contain: a pointer to a <b>std::string</b>.
 *
 */
class console{
    friend class drawing_manager;
    friend class console_test;
    private:
        void scroll(int scroll_y);
        void toggle();
        void show();
        void hide();
        void write(ST::console_log arg);
        void handle_messages();

        uint8_t log_level = 0x00;

        SDL_Color color{};
        SDL_Color color_text{};
        subscriber msg_sub{};
        bool shown = false;
        std::vector<ST::console_log> entries;
        int pos = 0;
        int font_size = 0;
        int scroll_offset = 0;

        //things to do with inputting text commands
        std::string composition;

        message_bus* gMessage_bus{};
public:
        console() = default;
        int initialize(message_bus* msg_bus);
        void set_log_level(ST::log_type arg);
        bool is_open();
        void update();
        void close();
};

//INLINED METHODS

/**
 * Consumes messages from the subscriber object.
 */
inline void console::update(){
    handle_messages();
}

#endif
