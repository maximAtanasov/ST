/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#include <console.hpp>
#include <key_definitions.hpp>

static bool singleton_initialized = false;

/**
 *
 * @param msg_bus A pointer to the global message bus.
 */
console::console(message_bus &gMessageBus) : gMessage_bus(gMessageBus) {
    if(singleton_initialized){
        throw std::runtime_error("The dev console cannot be initialized more than once!");
    }else{
        singleton_initialized = true;
    }

    color = {50, 50, 50, 100};
    color_text = {255, 255, 255, 255};
    color_info = {10, 50, 255, 255};
    color_error = {255, 0 ,0 , 255};
    color_success = {50, 255, 10, 255};
    shown = false;
    scroll_offset = 0;
    gMessage_bus.subscribe(LOG_ERROR, &msg_sub);
    gMessage_bus.subscribe(LOG_SUCCESS, &msg_sub);
    gMessage_bus.subscribe(LOG_INFO, &msg_sub);
    gMessage_bus.subscribe(CONSOLE_TOGGLE, &msg_sub);
    gMessage_bus.subscribe(MOUSE_SCROLL, &msg_sub);
    gMessage_bus.subscribe(KEY_PRESSED, &msg_sub);
	gMessage_bus.subscribe(KEY_HELD, &msg_sub);
    gMessage_bus.subscribe(KEY_RELEASED, &msg_sub);
    gMessage_bus.subscribe(TEXT_STREAM, &msg_sub);
    gMessage_bus.subscribe(CONSOLE_CLEAR, &msg_sub);
}

void console::post_init() const{
    gMessage_bus.send_msg(new message(REGISTER_KEY, static_cast<uint8_t>(ST::key::ENTER)));
    gMessage_bus.send_msg(new message(REGISTER_KEY, static_cast<uint8_t>(ST::key::TILDE)));
	gMessage_bus.send_msg(new message(REGISTER_KEY, static_cast<uint8_t>(ST::key::LEFT)));
	gMessage_bus.send_msg(new message(REGISTER_KEY, static_cast<uint8_t>(ST::key::RIGHT)));
	gMessage_bus.send_msg(new message(REGISTER_KEY, static_cast<uint8_t>(ST::key::UP)));
	gMessage_bus.send_msg(new message(REGISTER_KEY, static_cast<uint8_t>(ST::key::DOWN)));
	gMessage_bus.send_msg(new message(REGISTER_KEY, static_cast<uint8_t>(ST::key::BACKSPACE)));
	gMessage_bus.send_msg(new message(REGISTER_KEY, static_cast<uint8_t>(ST::key::DELETE)));
}


/**
 * @param scroll_y scrolls the console window relative to this amount.
 */
void console::scroll(int32_t scroll_y){
    scroll_offset = scroll_y*20;
}

/**
 * Consumes messages from the subscriber object and
 * performs the appropriate actions.
 */
void console::handle_messages(){
    message* temp = msg_sub.get_next_message();
    while(temp != nullptr){
        switch (temp->msg_name) {
            case LOG_ERROR: {
                auto log = static_cast<std::string*>(temp->get_data());
                if(log_level == 0x07 || log_level == 0x01 || log_level == 0x03 || log_level == 0x05) {
                    write(*log, ST::log_type::ERROR);
                }
                break;
            }
            case LOG_INFO: {
                auto log = static_cast<std::string*>(temp->get_data());
                if(log_level >= 0x04) {
                    write(*log, ST::log_type::INFO);
                }
                break;
            }
            case LOG_SUCCESS: {
                auto log = static_cast<std::string*>(temp->get_data());
                if(log_level >= 0x06 || log_level == 0x02 || log_level == 0x03) {
                    write(*log, ST::log_type::SUCCESS);
                }
                break;
            }
            case CONSOLE_TOGGLE:
                toggle();
                break;
            case CONSOLE_CLEAR:
                this->entries.clear();
                break;
            case MOUSE_SCROLL:
                scroll(static_cast<int32_t>(temp->base_data0));
                break;
            case KEY_HELD: {
                auto key_val = static_cast<ST::key>(temp->base_data0);
                if (is_open()) {
                    cursor_timer = 0; //Cursor won't blink when holding these keys
                    if(hold_counter > 10) { //These values can be changed to adjust the key hold delay.
                        hold_counter = 9;
                        if (key_val == ST::key::ENTER) {
                            enterKeyAction();
                        } else if (key_val == ST::key::LEFT) {
                            leftKeyAction();
                        } else if (key_val == ST::key::RIGHT) {
                            rightKeyAction();
                        } else if (key_val == ST::key::UP) {
                            upKeyAction();
                        } else if (key_val == ST::key::DOWN) {
                            downKeyAction();
                        } else if (key_val == ST::key::BACKSPACE) {
                            backspaceAction();
                        } else if (key_val == ST::key::DELETE) {
                            deleteKeyAction();
                        }
                    } else {
                        ++hold_counter;
                    }
                }
                break;
            }
            case KEY_PRESSED: {
                auto key_val = static_cast<ST::key>(temp->base_data0);
                if (key_val == ST::key::TILDE) {
                    toggle();
                } else if (is_open()) {
                    if (key_val == ST::key::ENTER) {
                        enterKeyAction();
                    } else if (key_val == ST::key::LEFT) {
                        leftKeyAction();
                    } else if (key_val == ST::key::RIGHT) {
                        rightKeyAction();
                    } else if (key_val == ST::key::UP) {
                        upKeyAction();
                    } else if (key_val == ST::key::DOWN) {
                        downKeyAction();
                    } else if (key_val == ST::key::BACKSPACE) {
                        backspaceAction();
                    } else if (key_val == ST::key::DELETE) {
                        deleteKeyAction();
                    }
                    hold_counter = 0;
                }
                break;
            }
            case TEXT_STREAM: {
                std::string received_data = *static_cast<std::string*>(temp->get_data());
                for(char const &c : received_data){
                    if(c > 126 || c < 0) {
                        received_data.clear();
                    }
                }
                if(received_data == "(") {
                    received_data += ")";
                    composition.insert(cursor_position--, received_data);
                } else if(received_data == "\"") {
                    received_data += received_data;
                    composition.insert(cursor_position--, received_data);
                } else if(received_data == "[" || received_data == "{") {
                    received_data += static_cast<char>(received_data.at(0) + 2);
                    composition.insert(cursor_position--, received_data);
                } else {
                    composition.insert(cursor_position, received_data);
                }
                cursor_position += static_cast<uint16_t>(received_data.size());
                gMessage_bus.send_msg(new message(CLEAR_TEXT_STREAM));
                break;
            }
        }
        delete temp;
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
        gMessage_bus.send_msg(new message(STOP_TEXT_INPUT));
    } else {
        show();
        gMessage_bus.send_msg(new message(START_TEXT_INPUT));
    }
}

/**
 * @param arg the text to write to the console window AND <b>stdout</b>.
 */
void console::write(const std::string &arg, ST::log_type type){
    if(type == ST::log_type::ERROR){
        fprintf(stderr, "%s\n", arg.c_str());
    } else {
        fprintf(stdout, "%s\n", arg.c_str());
    }
    entries.emplace_back(type, arg);
    //remove an entry if there are too many
    if(entries.size() > 1000) {
        entries.erase(entries.begin());
    }
}

/**
 *
 * @return bool indicating if the console window is open.
 */
bool console::is_open() const{
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
 */
console::~console(){
    singleton_initialized = false;
}

/**
 * Runs the actions associated with the backspace key.
 * Erases the last character of the current composition.
 */
void console::backspaceAction() {
    if (cursor_position > 0) {
        composition.erase(static_cast<uint16_t>(cursor_position - 1), 1);
        cursor_position -= 1;
    }
}

/**
 * Runs the actions associated with the right key.
 * Moves the cursor to the right.
 */
void console::rightKeyAction() {
    if (cursor_position < composition.size()) {
        cursor_position += 1;
    }
}

/**
 * Runs the actions associated with the left key.
 * Moves the cursor to the left.
 */
void console::leftKeyAction() {
    if (cursor_position > 0) {
        cursor_position -= 1;
    }
}

/**
 * Runs the actions associated with the down key.
 * Displays the next entry in the command history.
 */
void console::downKeyAction() {
    if (entries_history_index < command_entries.size() - 1) {
        entries_history_index++;
        composition = command_entries.at(static_cast<uint64_t>(entries_history_index));
    }
    else {
        entries_history_index = -1;
        composition = composition_history_temp;
    }
    gMessage_bus.send_msg(new message(CLEAR_TEXT_STREAM));
    cursor_position = static_cast<uint16_t>(composition.size());
}

/**
 * Runs the actions associated with the up key.
 * Displays the previous entry in the command history.
 */
void console::upKeyAction() {
    if (entries_history_index == -1 && !command_entries.empty()) {
        composition_history_temp = composition;
        composition = command_entries.back();
        entries_history_index = static_cast<int16_t>(command_entries.size() - 1);
    } else if (entries_history_index > 0) {
        entries_history_index--;
        composition = command_entries.at(static_cast<uint64_t>(entries_history_index));
    }
    gMessage_bus.send_msg(new message(CLEAR_TEXT_STREAM));
    cursor_position = static_cast<uint16_t>(composition.size());
}

/**
 * Runs the actions associated with the enter key.
 * Sends an EXECUTE_SCRIPT message with the current composition as it's data.
 * Writes the current composition to the console with log_type INFO.
 * Resets the cursor position and the history index.
 * Clears the text stream.
 */
void console::enterKeyAction() {
    if (!composition.empty()) {
        write(composition, ST::log_type::INFO);
        command_entries.emplace_back(composition);
        gMessage_bus.send_msg(new message(EXECUTE_SCRIPT, make_data(composition)));
    }
    composition.clear();
    cursor_position = 0;
    entries_history_index = -1;
    gMessage_bus.send_msg(new message(CLEAR_TEXT_STREAM));
}

/**
 * Runs the actions associated with the delete key.
 * Erases one character after the cursor position.
 */
void console::deleteKeyAction() {
    composition.erase(cursor_position, 1);
}

/**
 * Consumes messages from the subscriber object.
 */
void console::update(){
    handle_messages();
}

