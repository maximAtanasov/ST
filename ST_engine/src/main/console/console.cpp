/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <console/console.hpp>
#include <input_manager/key_definitions.hpp>

/**
 *
 * @param msg_bus A pointer to the global message bus.
 */
console::console(message_bus* msg_bus){
    gMessage_bus = msg_bus;
    color = {50, 50, 50, 100};
    color_text = {255, 255, 255, 255};
    shown = false;
    pos = 1080/2;
    font_size = 40;
    scroll_offset = 0;
    gMessage_bus->subscribe(CONSOLE_WRITE, &msg_sub);
    gMessage_bus->subscribe(CONSOLE_TOGGLE, &msg_sub);
    gMessage_bus->subscribe(MOUSE_SCROLL, &msg_sub);
    gMessage_bus->subscribe(KEY_PRESSED, &msg_sub);
    gMessage_bus->subscribe(TEXT_STREAM, &msg_sub);
    gMessage_bus->send_msg(make_msg(REGISTER_KEY, make_data<ST::key>(ST::key::ENTER)));
}

/**
 * @param scroll_y scrolls the console window relative to this amount.
 */
void console::scroll(int scroll_y){
    scroll_offset = scroll_y*20;
}

/**
 * Consumes messages from the subscriber object and
 * performs the appropriate actions.
 */
void console::handle_messages(){
    message* temp = msg_sub.get_next_message();
    while(temp != nullptr){
        if(temp->msg_name == CONSOLE_WRITE){
            auto log = (ST::console_log*)temp->get_data();
            if(log_level == 0x01) {
                if (log->type == ST::log_type::ERROR) {
                    write(*log);
                }
            }else if(log_level == 0x02){
                if(log->type == ST::log_type::SUCCESS){
                    write(*log);
                }
            }else if(log_level == 0x04){
                if(log->type == ST::log_type::INFO){
                    write(*log);
                }
            }else if(log_level == 0x03){
                if(log->type == ST::log_type::ERROR || log->type == ST::log_type::SUCCESS){
                    write(*log);
                }
            }else if(log_level == 0x05){
                if(log->type == ST::log_type::ERROR || log->type == ST::log_type::INFO){
                    write(*log);
                }
            }else if(log_level == 0x06){
                if(log->type == ST::log_type::SUCCESS || log->type == ST::log_type::INFO){
                    write(*log);
                }
            }else if(log_level == 0x07){
                write(*log);
            }
        }
        else if(temp->msg_name == CONSOLE_TOGGLE){
            toggle();
        }
        else if(temp->msg_name == MOUSE_SCROLL){
            int value = *(int*)temp->get_data();
            scroll(value);
        }
        else if(temp->msg_name == KEY_PRESSED){
            auto key_val = (ST::key*)temp->get_data();
            if(*key_val == ST::key::ENTER){
                write(ST::console_log(ST::log_type ::INFO, composition));
                if(!composition.empty()){
                    gMessage_bus->send_msg(make_msg(EXECUTE_SCRIPT, make_data<std::string>(composition)));
                }
                gMessage_bus->send_msg(make_msg(CLEAR_TEXT_STREAM, nullptr));
            }
        }
        else if(temp->msg_name == TEXT_STREAM){
            composition = *(std::string*)temp->get_data();
        }
        destroy_msg(temp);
        temp = msg_sub.get_next_message();
    }
}

/**
 * Set the log level.
 * @param arg The log level - this can be <b>log_type::ERROR</b>,  <b>log_type::INFO</b>,  <b>log_type::SUCCESS</b> or
 * any of the three or'ed together.
 *
 * For example:
 * <b>set_log_level(log_type::INFO | log_type::SUCCESS | log_type::ERROR)</b>;
 */
void console::set_log_level(ST::log_type arg) {
    log_level = static_cast<uint8_t>(arg);
}

/**
 * Toggles weather or not the console window is shown.
 */
void console::toggle() {
    if (is_open()) {
        hide();
        gMessage_bus->send_msg(make_msg(STOP_TEXT_INPUT, nullptr));
    } else {
        show();
        gMessage_bus->send_msg(make_msg(START_TEXT_INPUT, nullptr));
    }
}

/**
 * @param arg the <b>ST::console_log</b> object to write to the console window AND <b>stdout</b>.
 */
void console::write(ST::console_log arg){
    printf("%s\n", arg.text.c_str());
    entries.push_back(arg);
    //remove entries if there are too many
    if(entries.size() > 1000) {
        for (int i = 0; i < 900; i++) {
           entries.pop_back();
       }
    }
}

/**
 *
 * @return bool indicating if the console window is open.
 */
bool console::is_open(){
    return shown;
}

/**
 * hide the console window.
 */
void console::hide(){
    shown = false;
}

/**
 * show the console window.
 */
void console::show(){
    shown = true;
}

/**
 * Close the console.
 * Consume any leftover messages.
 */
console::~console(){
    handle_messages();
}


