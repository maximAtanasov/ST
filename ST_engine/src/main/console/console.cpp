#include <console/console.hpp>
#include <input_manager/key_definitions.hpp>

int console::initialize(message_bus* msg_bus){
    gMessage_bus = msg_bus;
    window =    {0,0, 1920, 1080/2};
    color = {50, 50, 50, 100};
    color_text = {255, 255, 255, 255};
    shown = false;
    pos = 1080/2;
    font_size = 40;
    scroll_offset = 0;
    msg_sub = new subscriber();
    gMessage_bus->subscribe(CONSOLE_WRITE, msg_sub);
    gMessage_bus->subscribe(CONSOLE_TOGGLE, msg_sub);
    gMessage_bus->subscribe(MOUSE_SCROLL, msg_sub);
    gMessage_bus->subscribe(KEY_PRESSED, msg_sub);
    gMessage_bus->subscribe(KEY_HELD, msg_sub);
    gMessage_bus->subscribe(KEY_RELEASED, msg_sub);
    gMessage_bus->subscribe(TEXT_STREAM, msg_sub);
    gMessage_bus->send_msg(make_msg(REGISTER_KEY, make_data<key>(key::ENTER)));
    return 0;
}

void console::scroll(int scroll_y){
    scroll_offset = scroll_y*20;
}

void console::update(){
    handle_messages();
}

void console::handle_messages(){
    message* temp = msg_sub->get_next_message();
    while(temp != nullptr){
        if(temp->msg_name == CONSOLE_WRITE){
            auto log = (console_log*)temp->get_data();
            if(log_level == 0x01) {
                if (log->type == log_type::ERROR) {
                    write(*log);
                }
            }else if(log_level == 0x02){
                if(log->type == log_type::SUCCESS){
                    write(*log);
                }
            }else if(log_level == 0x04){
                if(log->type == log_type::INFO){
                    write(*log);
                }
            }else if(log_level == 0x03){
                if(log->type == log_type::ERROR || log->type == log_type::SUCCESS){
                    write(*log);
                }
            }else if(log_level == 0x05){
                if(log->type == log_type::ERROR || log->type == log_type::INFO){
                    write(*log);
                }
            }else if(log_level == 0x06){
                if(log->type == log_type::SUCCESS || log->type == log_type::INFO){
                    write(*log);
                }
            }else if(log_level == 0x07){
                if(log->type == log_type::SUCCESS || log->type == log_type::INFO || log->type == log_type::ERROR){
                    write(*log);
                }
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
            auto key_val = (key*)temp->get_data();
            if(*key_val == key::ENTER){
                write(console_log(log_type ::INFO, composition));
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
        temp = msg_sub->get_next_message();
    }
}

void console::set_log_level(log_type arg) {
    log_level = (Uint8)arg;
    printf("%d\n", log_level);
}

void console::toggle() {
    if (is_open()) {
        hide();
        gMessage_bus->send_msg(make_msg(STOP_TEXT_INPUT, nullptr));
    } else {
        show();
        gMessage_bus->send_msg(make_msg(START_TEXT_INPUT, nullptr));
    }
}

void console::write(console_log arg){
    printf("%s\n", arg.text.c_str());
    entries.push_back(arg);
    //remove entries if there are too many
    if(entries.size() > 1000) {
        for (int i = 0; i < 900; i++) {
           entries.pop_back();
       }
    }
}

bool console::is_open(){
    return shown;
}

void console::hide(){
    shown = false;
}

void console::close(){
    handle_messages();
    delete msg_sub;
}

void console::show(){
    shown = true;
}
