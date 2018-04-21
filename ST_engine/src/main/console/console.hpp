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


class console{
    friend class drawing_manager;
    private:
        void scroll(int scroll_y);
        void toggle();
        void show();
        void hide();
        void write(ST::console_log arg);
        void handle_messages();

        Uint8 log_level = 0x00;

        SDL_Rect window{};
        SDL_Color color{};
        SDL_Color color_text{};
        subscriber* msg_sub{};
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
        void set_log_level(log_type arg);
        bool is_open();
        void update();
        void close();
};

//INLINED METHODS

inline void console::update(){
    handle_messages();
}

#endif
