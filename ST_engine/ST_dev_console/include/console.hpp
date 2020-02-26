/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#ifndef DEVCONSOLE_DEF
#define DEVCONSOLE_DEF

#include <message_bus.hpp>
#include "../src/main/console_log.hpp"

///This object represents the console window.
class console{
    friend class drawing_manager;
    friend class console_test;
    private:
        void scroll(int32_t scroll_y);
        void toggle();
        void show();
        void hide();
        void write(const std::string &arg, ST::log_type type);
        void handle_messages();

        void backspaceAction();
        void leftKeyAction();
        void rightKeyAction();
        void downKeyAction();
        void upKeyAction();
        void enterKeyAction();
        void deleteKeyAction();

        uint8_t log_level = 0x00;

        SDL_Color color{};
        SDL_Color color_text{};
        SDL_Color color_info{};
        SDL_Color color_error{};
        SDL_Color color_success{};


    subscriber msg_sub{};
        bool shown = false;
        std::vector<ST::console_log> entries;
		std::vector<std::string> command_entries;
        const uint8_t font_size = 40;
        int32_t scroll_offset = 0;

        //things to do with inputting text commands
		uint16_t cursor_position = 0;
		uint64_t cursor_timer = 0;
		int16_t entries_history_index = -1;
        std::string composition;
        std::string composition_history_temp;
        uint16_t hold_counter = 0;
		

        message_bus& gMessage_bus;
public:
        explicit console(message_bus &gMessageBus);
        ~console();
        void set_log_level(ST::log_type arg);
        [[nodiscard]] bool is_open() const;
        void update();
        void post_init() const;
};

//INLINED METHODS

/**
 * Consumes messages from the subscriber object.
 */
inline void console::update(){
    handle_messages();
}

#endif
