/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <console.hpp>
#include <key_definitions.hpp>

/**
 *
 * @param msg_bus A pointer to the global message bus.
 */
console::console(message_bus* msg_bus){
    gMessage_bus = msg_bus;
    color = {50, 50, 50, 100};
    color_text = {255, 255, 255, 255};
    shown = false;
    font_size = 40;
    scroll_offset = 0;
    gMessage_bus->subscribe(LOG_ERROR, &msg_sub);
    gMessage_bus->subscribe(LOG_SUCCESS, &msg_sub);
    gMessage_bus->subscribe(LOG_INFO, &msg_sub);
    gMessage_bus->subscribe(CONSOLE_TOGGLE, &msg_sub);
    gMessage_bus->subscribe(MOUSE_SCROLL, &msg_sub);
    gMessage_bus->subscribe(KEY_PRESSED, &msg_sub);
	gMessage_bus->subscribe(KEY_HELD, &msg_sub);
    gMessage_bus->subscribe(TEXT_STREAM, &msg_sub);
    gMessage_bus->subscribe(CONSOLE_CLEAR, &msg_sub);
}

void console::post_init() const{
    gMessage_bus->send_msg(make_msg(REGISTER_KEY, make_data(ST::key::ENTER)));
    gMessage_bus->send_msg(make_msg(REGISTER_KEY, make_data(ST::key::TILDE)));
	gMessage_bus->send_msg(make_msg(REGISTER_KEY, make_data(ST::key::LEFT)));
	gMessage_bus->send_msg(make_msg(REGISTER_KEY, make_data(ST::key::RIGHT)));
	gMessage_bus->send_msg(make_msg(REGISTER_KEY, make_data(ST::key::UP)));
	gMessage_bus->send_msg(make_msg(REGISTER_KEY, make_data(ST::key::DOWN)));
	gMessage_bus->send_msg(make_msg(REGISTER_KEY, make_data(ST::key::BACKSPACE)));
	gMessage_bus->send_msg(make_msg(REGISTER_KEY, make_data(ST::key::DELETE)));

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
        if(temp->msg_name == LOG_ERROR){
            auto log = static_cast<std::string*>(temp->get_data());
            if(log_level == 0x01 || log_level >= 0x03) {
                write(*log, ST::log_type::ERROR);
            }
        }else if(temp->msg_name == LOG_INFO){
            auto log = static_cast<std::string*>(temp->get_data());
            if(log_level == 0x04 || log_level >= 0x05) {
                write(*log, ST::log_type::INFO);
            }
        }else if(temp->msg_name == LOG_SUCCESS){
            auto log = static_cast<std::string*>(temp->get_data());
            if(log_level == 0x02 || log_level == 0x03 || log_level > 0x06) {
                write(*log, ST::log_type::SUCCESS);
            }
        }
        else if(temp->msg_name == CONSOLE_TOGGLE){
            toggle();
        }
        else if(temp->msg_name == CONSOLE_CLEAR){
            this->entries.clear();
        }
        else if(temp->msg_name == MOUSE_SCROLL){
            int value = *static_cast<int32_t*>(temp->get_data());
            scroll(value);
        }
        else if(temp->msg_name == KEY_PRESSED){
            auto key_val = static_cast<ST::key*>(temp->get_data());
            if(*key_val == ST::key::ENTER){
                if(!composition.empty()){
                    write(composition, ST::log_type ::INFO);
                    gMessage_bus->send_msg(make_msg(EXECUTE_SCRIPT, make_data(composition)));
                }
                composition.clear();
				cursor_position = 0;
				entries_history_index = -1;
                gMessage_bus->send_msg(make_msg(CLEAR_TEXT_STREAM, nullptr));
            }
			else if(*key_val == ST::key::TILDE) {
                toggle();
            }
			else if (*key_val == ST::key::LEFT) {
				if (cursor_position > 0) {
					cursor_position -= 1;
				}
			}
			else if (*key_val == ST::key::RIGHT) {
				if (cursor_position < composition.size()) {
					cursor_position += 1;
				}
			}
			else if (*key_val == ST::key::UP) {
				if (entries_history_index == -1) {
					composition = entries.back().text;
					entries_history_index = entries.size() - 1;
				}
				else if(entries_history_index > 0){
					entries_history_index--;
					composition = entries.at(entries_history_index).text;
				}
				gMessage_bus->send_msg(make_msg(CLEAR_TEXT_STREAM, nullptr));
				cursor_position = composition.size();
			}
			else if (*key_val == ST::key::DOWN) {
				if (entries_history_index < entries.size()-1) {
					entries_history_index++;
					composition = entries.at(entries_history_index).text;
				}
				else {
					entries_history_index = -1;
					composition = "";
				}
				gMessage_bus->send_msg(make_msg(CLEAR_TEXT_STREAM, nullptr));
				cursor_position = composition.size();
			}
			else if (*key_val == ST::key::BACKSPACE) {
				if (cursor_position > 0) {
					composition.erase(cursor_position - 1, 1);
					cursor_position -= 1;
				}
			}
			else if (*key_val == ST::key::DELETE) {
				composition.erase(cursor_position, 1);
			}
        }
		else if (temp->msg_name == KEY_HELD) {
			auto key_val = static_cast<ST::key*>(temp->get_data());
			
		}
        else if(temp->msg_name == TEXT_STREAM){
			std::string recieved_data = *static_cast<std::string*>(temp->get_data());
			if (composition.size() == cursor_position) {
				composition += recieved_data;
			}
			else {
				composition.insert(cursor_position, recieved_data);
			}
			cursor_position += recieved_data.size();
			gMessage_bus->send_msg(make_msg(CLEAR_TEXT_STREAM, nullptr));
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
void console::write(const std::string &arg, ST::log_type type){
    printf("%s\n", arg.c_str());
    entries.emplace_back(type, arg);
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
 * Consume any leftover messages.
 */
console::~console(){
    handle_messages();
}


