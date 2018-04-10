#include <input_manager/input_manager.hpp>

int input_manager::initialize(message_bus* msg_bus, task_manager* tsk_mngr){

    //SET OUR EXTERNAL DEPENDENCIES
    gMessage_bus = msg_bus;
    gTask_manager = tsk_mngr;

    //Initialize controls
    int length = 0;
    controls.keyboard = SDL_GetKeyboardState(&length);
    controls.keyboardFramePrev = (Uint8*)malloc(sizeof(Uint8)*length);
    SDL_PollEvent(&event);
    controls.mouseX = 0;
    controls.mouseY = 0;
    controls.mouseX_prev = controls.mouseX;
    controls.mouseY_prev = controls.mouseY;
    controls.mouse_scroll = 0;

    //Create a subscriber object
	msg_sub = new subscriber();

    //Subscribe to the messages we need
	gMessage_bus->subscribe(VIRTUAL_SCREEN_COORDINATES, msg_sub);
	gMessage_bus->subscribe(REAL_SCREEN_COORDINATES, msg_sub);
    gMessage_bus->subscribe(START_TEXT_INPUT, msg_sub);
    gMessage_bus->subscribe(STOP_TEXT_INPUT, msg_sub);
    gMessage_bus->subscribe(CLEAR_TEXT_STREAM, msg_sub);
    gMessage_bus->subscribe(REGISTER_KEY, msg_sub);
    //gMessage_bus->subscribe(UNREGISTER_KEY, msg_sub);

    return 0;
}

void input_manager::update_task(void* mngr){
    auto self = (input_manager*)mngr;
    self->handle_messages();
    self->take_input();
}

void input_manager::update(){
    gTask_manager->start_task_lockfree(new task(update_task, this, nullptr, -1));
}

void input_manager::take_input(){
    int length = 0;
    SDL_GetKeyboardState(&length);
    for(int i = 0; i < length; i++){
        controls.keyboardFramePrev[i] = controls.keyboard[i];
    }

    //Collect mouse input
	controls.mouseClicksFramePrev[0] = controls.mouseClicks[0];
	controls.mouseClicksFramePrev[1] = controls.mouseClicks[1];
	controls.mouseClicksFramePrev[2] = controls.mouseClicks[2];
    for(int& i : controls.mouseClicks){
        i = 0;
    }
    if(SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT))
        controls.mouseClicks[0] = 1;
    if(SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_MIDDLE))
        controls.mouseClicks[1] = 1;
    if(SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_RIGHT))
        controls.mouseClicks[2] = 1;

    while(SDL_PollEvent(&event) != 0){
        if(event.type == SDL_QUIT){
            gMessage_bus->send_msg(make_msg(END_GAME, nullptr));
        }
        if(event.type == SDL_MOUSEWHEEL){
            controls.mouse_scroll += event.wheel.y;
            if(controls.mouse_scroll < 0){
                controls.mouse_scroll = 0;
            }
            gMessage_bus->send_msg(make_msg(MOUSE_SCROLL, make_data<int>(controls.mouse_scroll)));
        }
        if(event.type == SDL_TEXTINPUT){
            if(text_input) {
                composition += event.edit.text;

                #ifdef __DEBUG
                //Drop the tilde when closing the console, otherwise it just stays there
                if(!composition.empty() && composition.at(composition.size()-1) == CONSOLE_TOGGLE_KEY){
                    composition.pop_back();
                }
                #endif
                gMessage_bus->send_msg(make_msg(TEXT_STREAM, make_data<std::string>(composition)));
            }
        }
        if(event.type == SDL_KEYDOWN){
            if(event.key.keysym.sym == SDLK_BACKSPACE) {
                if (composition.length() > 0) {
                    composition.pop_back();
                    gMessage_bus->send_msg(make_msg(TEXT_STREAM, make_data<std::string>(composition)));
                }
            }
        }
		if(event.type == SDL_MOUSEMOTION){
			SDL_GetMouseState(&controls.mouseX, &controls.mouseY);
			controls.mouseX = static_cast<int>(static_cast<float>(controls.mouseX)*ratio_w);
			controls.mouseY = static_cast<int>(static_cast<float>(controls.mouseY)*ratio_h);
		}
    }



    //check if any of the registered keys is pressed and send a message if so
    for(auto i : registered_keys){
        if(keypress(i)){
            gMessage_bus->send_msg(make_msg(KEY_PRESSED, make_data<Uint8>((Uint8)i)));
        }
        else if(keyheld(i)){
            gMessage_bus->send_msg(make_msg(KEY_HELD, make_data<Uint8>((Uint8)i)));
        }
        else if(keyrelease(i)){
            gMessage_bus->send_msg(make_msg(KEY_RELEASED, make_data<Uint8>((Uint8)i)));
        }
    }

    #ifdef __DEBUG
    if(keypress(key::TILDE)){
        gMessage_bus->send_msg(make_msg(CONSOLE_TOGGLE, nullptr));
    }
    #endif

    //only send mouse coordinates if they change
    if(controls.mouseX != controls.mouseX_prev){
        gMessage_bus->send_msg(make_msg(MOUSE_X, make_data<int>(controls.mouseX)));
        controls.mouseX_prev = controls.mouseX;
    }
    if(controls.mouseY != controls.mouseY_prev){
        gMessage_bus->send_msg(make_msg(MOUSE_Y, make_data<int>(controls.mouseY)));
        controls.mouseY_prev = controls.mouseY;
    }
}

void input_manager::handle_messages(){
	message* temp = msg_sub->get_next_message();
	while(temp != nullptr){
		if(temp->msg_name == VIRTUAL_SCREEN_COORDINATES){
			auto data = (std::tuple<int, int>*)temp->get_data();
            v_width = std::get<0> (*data);
            v_height = std::get<1> (*data);
			ratio_w = (float)v_width/(float)r_width;
			ratio_h = (float)v_height/(float)r_height;
		}else if(temp->msg_name == REAL_SCREEN_COORDINATES) {
            auto data = (std::tuple<int, int> *) temp->get_data();
            r_width = std::get<0>(*data);
            r_height = std::get<1>(*data);
            ratio_w = (float) v_width / (float) r_width;
            ratio_h = (float) v_height / (float) r_height;
        }else if(temp->msg_name == START_TEXT_INPUT){
            text_input = true;
        }else if(temp->msg_name == STOP_TEXT_INPUT){
            text_input = false;
        }else if(temp->msg_name == CLEAR_TEXT_STREAM){
            composition.clear();
            gMessage_bus->send_msg(make_msg(TEXT_STREAM, make_data<std::string>(composition)));
        }else if(temp->msg_name == REGISTER_KEY){
            auto key_val = (key*)temp->get_data();
            registered_keys.push_back(*key_val);
        }else if(temp->msg_name == UNREGISTER_KEY){
            auto key_val = (key*)temp->get_data();
            registered_keys.erase(std::remove(registered_keys.begin(), registered_keys.end(), *key_val), registered_keys.end());
        }
		destroy_msg(temp);
		temp = msg_sub->get_next_message();
	}
}

bool input_manager::keypress(key arg){
    bool pressed = false;
    if(arg == key::LEFT){
        if(controls.keyboard[SDL_SCANCODE_LEFT] && !controls.keyboardFramePrev[SDL_SCANCODE_LEFT])
            pressed = true;
    }
    else if(arg == key::RIGHT){
        if(controls.keyboard[SDL_SCANCODE_RIGHT] && !controls.keyboardFramePrev[SDL_SCANCODE_RIGHT])
            pressed = true;
    }
    else if(arg == key::UP){
        if(controls.keyboard[SDL_SCANCODE_UP] && !controls.keyboardFramePrev[SDL_SCANCODE_UP])
            pressed = true;
    }
    else if(arg == key::DOWN){
        if(controls.keyboard[SDL_SCANCODE_DOWN] && !controls.keyboardFramePrev[SDL_SCANCODE_DOWN])
            pressed = true;
    }
    else if(arg == key::A){
        if(controls.keyboard[SDL_SCANCODE_A] && !controls.keyboardFramePrev[SDL_SCANCODE_A])
            pressed = true;
    }
    else if(arg == key::B){
        if(controls.keyboard[SDL_SCANCODE_B] && !controls.keyboardFramePrev[SDL_SCANCODE_B])
            pressed = true;
    }
    else if(arg == key::C){
        if(controls.keyboard[SDL_SCANCODE_C] && !controls.keyboardFramePrev[SDL_SCANCODE_C])
            pressed = true;
    }
    else if(arg == key::D){
        if(controls.keyboard[SDL_SCANCODE_D] && !controls.keyboardFramePrev[SDL_SCANCODE_D])
            pressed = true;
    }
    else if(arg == key::E){
        if(controls.keyboard[SDL_SCANCODE_E] && !controls.keyboardFramePrev[SDL_SCANCODE_E])
            pressed = true;
    }
    else if(arg == key::F){
        if(controls.keyboard[SDL_SCANCODE_F] && !controls.keyboardFramePrev[SDL_SCANCODE_F])
            pressed = true;
    }
    else if(arg == key::G){
        if(controls.keyboard[SDL_SCANCODE_G] && !controls.keyboardFramePrev[SDL_SCANCODE_G])
            pressed = true;
    }
    else if(arg == key::H){
        if(controls.keyboard[SDL_SCANCODE_H] && !controls.keyboardFramePrev[SDL_SCANCODE_H])
            pressed = true;
    }
    else if(arg == key::I){
        if(controls.keyboard[SDL_SCANCODE_I] && !controls.keyboardFramePrev[SDL_SCANCODE_I])
            pressed = true;
    }
    else if(arg == key::J){
        if(controls.keyboard[SDL_SCANCODE_J] && !controls.keyboardFramePrev[SDL_SCANCODE_G])
            pressed = true;
    }
    else if(arg == key::K){
        if(controls.keyboard[SDL_SCANCODE_K] && !controls.keyboardFramePrev[SDL_SCANCODE_K])
            pressed = true;
    }
    else if(arg == key::L){
        if(controls.keyboard[SDL_SCANCODE_L] && !controls.keyboardFramePrev[SDL_SCANCODE_L])
            pressed = true;
    }
    else if(arg == key::M){
        if(controls.keyboard[SDL_SCANCODE_M] && !controls.keyboardFramePrev[SDL_SCANCODE_M])
            pressed = true;
    }
    else if(arg == key::N){
        if(controls.keyboard[SDL_SCANCODE_N] && !controls.keyboardFramePrev[SDL_SCANCODE_N])
            pressed = true;
    }
    else if(arg == key::O){
        if(controls.keyboard[SDL_SCANCODE_O] && !controls.keyboardFramePrev[SDL_SCANCODE_O])
            pressed = true;
    }
    else if(arg == key::P){
        if(controls.keyboard[SDL_SCANCODE_P] && !controls.keyboardFramePrev[SDL_SCANCODE_P])
            pressed = true;
    }
    else if(arg == key::Q){
        if(controls.keyboard[SDL_SCANCODE_Q] && !controls.keyboardFramePrev[SDL_SCANCODE_Q])
            pressed = true;
    }
    else if(arg == key::R){
        if(controls.keyboard[SDL_SCANCODE_R] && !controls.keyboardFramePrev[SDL_SCANCODE_R])
            pressed = true;
    }
    else if(arg == key::S){
        if(controls.keyboard[SDL_SCANCODE_S] && !controls.keyboardFramePrev[SDL_SCANCODE_S])
            pressed = true;
    }
    else if(arg == key::T){
        if(controls.keyboard[SDL_SCANCODE_T] && !controls.keyboardFramePrev[SDL_SCANCODE_T])
            pressed = true;
    }
    else if(arg == key::U){
        if(controls.keyboard[SDL_SCANCODE_U] && !controls.keyboardFramePrev[SDL_SCANCODE_U])
            pressed = true;
    }
    else if(arg == key::V){
        if(controls.keyboard[SDL_SCANCODE_V] && !controls.keyboardFramePrev[SDL_SCANCODE_V])
            pressed = true;
    }
    else if(arg == key::W){
        if(controls.keyboard[SDL_SCANCODE_W] && !controls.keyboardFramePrev[SDL_SCANCODE_W])
            pressed = true;
    }
    else if(arg == key::X){
        if(controls.keyboard[SDL_SCANCODE_X] && !controls.keyboardFramePrev[SDL_SCANCODE_X])
            pressed = true;
    }
    else if(arg == key::Y){
        if(controls.keyboard[SDL_SCANCODE_Y] && !controls.keyboardFramePrev[SDL_SCANCODE_Y])
            pressed = true;
    }
    else if(arg == key::Z){
        if(controls.keyboard[SDL_SCANCODE_Z] && !controls.keyboardFramePrev[SDL_SCANCODE_Z])
            pressed = true;
    }
    else if(arg == key::ONE){
        if(controls.keyboard[SDL_SCANCODE_1] && !controls.keyboardFramePrev[SDL_SCANCODE_1])
            pressed = true;
    }
    else if(arg == key::TWO){
        if(controls.keyboard[SDL_SCANCODE_2] && !controls.keyboardFramePrev[SDL_SCANCODE_2])
            pressed = true;
    }
    else if(arg == key::THREE){
        if(controls.keyboard[SDL_SCANCODE_3] && !controls.keyboardFramePrev[SDL_SCANCODE_3])
            pressed = true;
    }
    else if(arg == key::FOUR){
        if(controls.keyboard[SDL_SCANCODE_4] && !controls.keyboardFramePrev[SDL_SCANCODE_4])
            pressed = true;
    }
    else if(arg == key::FIVE){
        if(controls.keyboard[SDL_SCANCODE_5] && !controls.keyboardFramePrev[SDL_SCANCODE_5])
            pressed = true;
    }
    else if(arg == key::SIX){
        if(controls.keyboard[SDL_SCANCODE_6] && !controls.keyboardFramePrev[SDL_SCANCODE_6])
            pressed = true;
    }
    else if(arg == key::SEVEN){
        if(controls.keyboard[SDL_SCANCODE_7] && !controls.keyboardFramePrev[SDL_SCANCODE_7])
            pressed = true;
    }
    else if(arg == key::EIGHT){
        if(controls.keyboard[SDL_SCANCODE_8] && !controls.keyboardFramePrev[SDL_SCANCODE_8])
            pressed = true;
    }
    else if(arg == key::NINE){
        if(controls.keyboard[SDL_SCANCODE_9] && !controls.keyboardFramePrev[SDL_SCANCODE_9])
            pressed = true;
    }
    else if(arg == key::ZERO){
        if(controls.keyboard[SDL_SCANCODE_0] && !controls.keyboardFramePrev[SDL_SCANCODE_0])
            pressed = true;
    }
    else if(arg == key::ESCAPE){
        if(controls.keyboard[SDL_SCANCODE_ESCAPE] && !controls.keyboardFramePrev[SDL_SCANCODE_ESCAPE])
            pressed = true;
    }
    else if(arg == key::ENTER){
        if(controls.keyboard[SDL_SCANCODE_RETURN] && !controls.keyboardFramePrev[SDL_SCANCODE_RETURN])
            pressed = true;
    }
    else if(arg == key::SPACEBAR){
        if(controls.keyboard[SDL_SCANCODE_SPACE] && !controls.keyboardFramePrev[SDL_SCANCODE_SPACE])
            pressed = true;
    }
    else if(arg == key::TILDE){
        if(controls.keyboard[SDL_SCANCODE_GRAVE] && !controls.keyboardFramePrev[SDL_SCANCODE_GRAVE])
            pressed = true;
    }
    else if(arg == key::LSHIFT){
        if(controls.keyboard[SDL_SCANCODE_LSHIFT] && !controls.keyboardFramePrev[SDL_SCANCODE_LSHIFT])
            pressed = true;
    }
    else if(arg == key::BACKSPACE){
        if(controls.keyboard[SDL_SCANCODE_BACKSPACE] && !controls.keyboardFramePrev[SDL_SCANCODE_BACKSPACE])
            pressed = true;
    }
    else if(arg == key::BACKSLASH){
        if(controls.keyboard[SDL_SCANCODE_BACKSLASH] && !controls.keyboardFramePrev[SDL_SCANCODE_BACKSLASH])
            pressed = true;
    }
    else if(arg == key::CAPSLOCK){
        if(controls.keyboard[SDL_SCANCODE_CAPSLOCK] && !controls.keyboardFramePrev[SDL_SCANCODE_CAPSLOCK])
            pressed = true;
    }
    else if(arg == key::COMMA){
        if(controls.keyboard[SDL_SCANCODE_COMMA] && !controls.keyboardFramePrev[SDL_SCANCODE_COMMA])
            pressed = true;
    }
    else if(arg == key::EQUALS){
        if(controls.keyboard[SDL_SCANCODE_EQUALS] && !controls.keyboardFramePrev[SDL_SCANCODE_EQUALS])
            pressed = true;
    }
    else if(arg == key::LALT){
        if(controls.keyboard[SDL_SCANCODE_LALT] && !controls.keyboardFramePrev[SDL_SCANCODE_LALT])
            pressed = true;
    }
    else if(arg == key::LCTRL){
        if(controls.keyboard[SDL_SCANCODE_LCTRL] && !controls.keyboardFramePrev[SDL_SCANCODE_LCTRL])
            pressed = true;
    }
    else if(arg == key::LBRACKET){
        if(controls.keyboard[SDL_SCANCODE_LEFTBRACKET] && !controls.keyboardFramePrev[SDL_SCANCODE_LEFTBRACKET])
            pressed = true;
    }
    else if(arg == key::RBRACKET){
        if(controls.keyboard[SDL_SCANCODE_RIGHTBRACKET] && !controls.keyboardFramePrev[SDL_SCANCODE_RIGHTBRACKET])
            pressed = true;
    }
    else if(arg == key::MINUS){
        if(controls.keyboard[SDL_SCANCODE_MINUS] && !controls.keyboardFramePrev[SDL_SCANCODE_MINUS])
            pressed = true;
    }
    else if(arg == key::RALT){
        if(controls.keyboard[SDL_SCANCODE_RALT] && !controls.keyboardFramePrev[SDL_SCANCODE_RALT])
            pressed = true;
    }
    else if(arg == key::RCTRL){
        if(controls.keyboard[SDL_SCANCODE_RCTRL] && !controls.keyboardFramePrev[SDL_SCANCODE_RCTRL])
            pressed = true;
    }
    else if(arg == key::SEMICOLON){
        if(controls.keyboard[SDL_SCANCODE_SEMICOLON] && !controls.keyboardFramePrev[SDL_SCANCODE_SEMICOLON])
            pressed = true;
    }
    else if(arg == key::SLASH){
        if(controls.keyboard[SDL_SCANCODE_SLASH] && !controls.keyboardFramePrev[SDL_SCANCODE_SLASH])
            pressed = true;
    }
    else if(arg == key::TAB){
        if(controls.keyboard[SDL_SCANCODE_TAB] && !controls.keyboardFramePrev[SDL_SCANCODE_TAB])
            pressed = true;
    }
    else if(arg == key::MOUSELEFT){
        if(!controls.mouseClicksFramePrev[0] && controls.mouseClicks[0])
            pressed = true;
    }
    else if(arg == key::MOUSEMIDDLE){
        if(!controls.mouseClicksFramePrev[1] && controls.mouseClicks[1])
            pressed = true;
    }
    else if(arg == key::MOUSERIGHT){
        if(!controls.mouseClicksFramePrev[2] && controls.mouseClicks[2])
            pressed = true;
    }
    return pressed;
}

bool input_manager::keyheld(key arg){
    bool held = false;
    if(arg == key::LEFT){
        if(controls.keyboard[SDL_SCANCODE_LEFT] && controls.keyboardFramePrev[SDL_SCANCODE_LEFT])
            held = true;
    }
    else if(arg == key::RIGHT){
        if(controls.keyboard[SDL_SCANCODE_RIGHT] && controls.keyboardFramePrev[SDL_SCANCODE_RIGHT])
            held = true;
    }
    else if(arg == key::UP){
        if(controls.keyboard[SDL_SCANCODE_UP] && controls.keyboardFramePrev[SDL_SCANCODE_UP])
            held = true;
    }
    else if(arg == key::DOWN){
        if(controls.keyboard[SDL_SCANCODE_DOWN] && controls.keyboardFramePrev[SDL_SCANCODE_DOWN])
            held = true;
    }
    else if(arg == key::A){
        if(controls.keyboard[SDL_SCANCODE_A] && controls.keyboardFramePrev[SDL_SCANCODE_A])
            held = true;
    }
    else if(arg == key::B){
        if(controls.keyboard[SDL_SCANCODE_B] && controls.keyboardFramePrev[SDL_SCANCODE_B])
            held = true;
    }
    else if(arg == key::C){
        if(controls.keyboard[SDL_SCANCODE_C] && controls.keyboardFramePrev[SDL_SCANCODE_C])
            held = true;
    }
    else if(arg == key::D){
        if(controls.keyboard[SDL_SCANCODE_D] && controls.keyboardFramePrev[SDL_SCANCODE_D])
            held = true;
    }
    else if(arg == key::E){
        if(controls.keyboard[SDL_SCANCODE_E] && controls.keyboardFramePrev[SDL_SCANCODE_E])
            held = true;
    }
    else if(arg == key::F){
        if(controls.keyboard[SDL_SCANCODE_F] && controls.keyboardFramePrev[SDL_SCANCODE_F])
            held = true;
    }
    else if(arg == key::G){
        if(controls.keyboard[SDL_SCANCODE_G] && controls.keyboardFramePrev[SDL_SCANCODE_G])
            held = true;
    }
    else if(arg == key::H){
        if(controls.keyboard[SDL_SCANCODE_H] && controls.keyboardFramePrev[SDL_SCANCODE_H])
            held = true;
    }
    else if(arg == key::I){
        if(controls.keyboard[SDL_SCANCODE_I] && controls.keyboardFramePrev[SDL_SCANCODE_I])
            held = true;
    }
    else if(arg == key::J){
        if(controls.keyboard[SDL_SCANCODE_J] && controls.keyboardFramePrev[SDL_SCANCODE_G])
            held = true;
    }
    else if(arg == key::K){
        if(controls.keyboard[SDL_SCANCODE_K] && controls.keyboardFramePrev[SDL_SCANCODE_K])
            held = true;
    }
    else if(arg == key::L){
        if(controls.keyboard[SDL_SCANCODE_L] && controls.keyboardFramePrev[SDL_SCANCODE_L])
            held = true;
    }
    else if(arg == key::M){
        if(controls.keyboard[SDL_SCANCODE_M] && controls.keyboardFramePrev[SDL_SCANCODE_M])
            held = true;
    }
    else if(arg == key::N){
        if(controls.keyboard[SDL_SCANCODE_N] && controls.keyboardFramePrev[SDL_SCANCODE_N])
            held = true;
    }
    else if(arg == key::O){
        if(controls.keyboard[SDL_SCANCODE_O] && controls.keyboardFramePrev[SDL_SCANCODE_O])
            held = true;
    }
    else if(arg == key::P){
        if(controls.keyboard[SDL_SCANCODE_P] && controls.keyboardFramePrev[SDL_SCANCODE_P])
            held = true;
    }
    else if(arg == key::Q){
        if(controls.keyboard[SDL_SCANCODE_Q] && controls.keyboardFramePrev[SDL_SCANCODE_Q])
            held = true;
    }
    else if(arg == key::R){
        if(controls.keyboard[SDL_SCANCODE_R] && controls.keyboardFramePrev[SDL_SCANCODE_R])
            held = true;
    }
    else if(arg == key::S){
        if(controls.keyboard[SDL_SCANCODE_S] && controls.keyboardFramePrev[SDL_SCANCODE_S])
            held = true;
    }
    else if(arg == key::T){
        if(controls.keyboard[SDL_SCANCODE_T] && controls.keyboardFramePrev[SDL_SCANCODE_T])
            held = true;
    }
    else if(arg == key::U){
        if(controls.keyboard[SDL_SCANCODE_U] && controls.keyboardFramePrev[SDL_SCANCODE_U])
            held = true;
    }
    else if(arg == key::V){
        if(controls.keyboard[SDL_SCANCODE_V] && controls.keyboardFramePrev[SDL_SCANCODE_V])
            held = true;
    }
    else if(arg == key::W){
        if(controls.keyboard[SDL_SCANCODE_W] && controls.keyboardFramePrev[SDL_SCANCODE_W])
            held = true;
    }
    else if(arg == key::X){
        if(controls.keyboard[SDL_SCANCODE_X] && controls.keyboardFramePrev[SDL_SCANCODE_X])
            held = true;
    }
    else if(arg == key::Y){
        if(controls.keyboard[SDL_SCANCODE_Y] && controls.keyboardFramePrev[SDL_SCANCODE_Y])
            held = true;
    }
    else if(arg == key::Z){
        if(controls.keyboard[SDL_SCANCODE_Z] && controls.keyboardFramePrev[SDL_SCANCODE_Z])
            held = true;
    }
    else if(arg == key::ONE){
        if(controls.keyboard[SDL_SCANCODE_1] && controls.keyboardFramePrev[SDL_SCANCODE_1])
            held = true;
    }
    else if(arg == key::TWO){
        if(controls.keyboard[SDL_SCANCODE_2] && controls.keyboardFramePrev[SDL_SCANCODE_2])
            held = true;
    }
    else if(arg == key::THREE){
        if(controls.keyboard[SDL_SCANCODE_3] && controls.keyboardFramePrev[SDL_SCANCODE_3])
            held = true;
    }
    else if(arg == key::FOUR){
        if(controls.keyboard[SDL_SCANCODE_4] && controls.keyboardFramePrev[SDL_SCANCODE_4])
            held = true;
    }
    else if(arg == key::FIVE){
        if(controls.keyboard[SDL_SCANCODE_5] && controls.keyboardFramePrev[SDL_SCANCODE_5])
            held = true;
    }
    else if(arg == key::SIX){
        if(controls.keyboard[SDL_SCANCODE_6] && controls.keyboardFramePrev[SDL_SCANCODE_6])
            held = true;
    }
    else if(arg == key::SEVEN){
        if(controls.keyboard[SDL_SCANCODE_7] && controls.keyboardFramePrev[SDL_SCANCODE_7])
            held = true;
    }
    else if(arg == key::EIGHT){
        if(controls.keyboard[SDL_SCANCODE_8] && controls.keyboardFramePrev[SDL_SCANCODE_8])
            held = true;
    }
    else if(arg == key::NINE){
        if(controls.keyboard[SDL_SCANCODE_9] && controls.keyboardFramePrev[SDL_SCANCODE_9])
            held = true;
    }
    else if(arg == key::ZERO){
        if(controls.keyboard[SDL_SCANCODE_0] && controls.keyboardFramePrev[SDL_SCANCODE_0])
            held = true;
    }
    else if(arg == key::ESCAPE){
        if(controls.keyboard[SDL_SCANCODE_ESCAPE] && controls.keyboardFramePrev[SDL_SCANCODE_ESCAPE])
            held = true;
    }
    else if(arg == key::ENTER){
        if(controls.keyboard[SDL_SCANCODE_RETURN] && controls.keyboardFramePrev[SDL_SCANCODE_RETURN])
            held = true;
    }
    else if(arg == key::SPACEBAR){
        if(controls.keyboard[SDL_SCANCODE_SPACE] && controls.keyboardFramePrev[SDL_SCANCODE_SPACE])
            held = true;
    }
    else if(arg == key::TILDE){
        if(controls.keyboard[SDL_SCANCODE_GRAVE] && controls.keyboardFramePrev[SDL_SCANCODE_GRAVE])
            held = true;
    }
    else if(arg == key::LSHIFT){
        if(controls.keyboard[SDL_SCANCODE_LSHIFT] && controls.keyboardFramePrev[SDL_SCANCODE_LSHIFT])
            held = true;
    }
    else if(arg == key::BACKSPACE){
        if(controls.keyboard[SDL_SCANCODE_BACKSPACE] && controls.keyboardFramePrev[SDL_SCANCODE_BACKSPACE])
            held = true;
    }
    else if(arg == key::BACKSLASH){
        if(controls.keyboard[SDL_SCANCODE_BACKSLASH] && controls.keyboardFramePrev[SDL_SCANCODE_BACKSLASH])
            held = true;
    }
    else if(arg == key::CAPSLOCK){
        if(controls.keyboard[SDL_SCANCODE_CAPSLOCK] && controls.keyboardFramePrev[SDL_SCANCODE_CAPSLOCK])
            held = true;
    }
    else if(arg == key::COMMA){
        if(controls.keyboard[SDL_SCANCODE_COMMA] && controls.keyboardFramePrev[SDL_SCANCODE_COMMA])
            held = true;
    }
    else if(arg == key::EQUALS){
        if(controls.keyboard[SDL_SCANCODE_EQUALS] && controls.keyboardFramePrev[SDL_SCANCODE_EQUALS])
            held = true;
    }
    else if(arg == key::LALT){
        if(controls.keyboard[SDL_SCANCODE_LALT] && controls.keyboardFramePrev[SDL_SCANCODE_LALT])
            held = true;
    }
    else if(arg == key::LCTRL){
        if(controls.keyboard[SDL_SCANCODE_LCTRL] && controls.keyboardFramePrev[SDL_SCANCODE_LCTRL])
            held = true;
    }
    else if(arg == key::LBRACKET){
        if(controls.keyboard[SDL_SCANCODE_LEFTBRACKET] && controls.keyboardFramePrev[SDL_SCANCODE_LEFTBRACKET])
            held = true;
    }
    else if(arg == key::RBRACKET){
        if(controls.keyboard[SDL_SCANCODE_RIGHTBRACKET] && controls.keyboardFramePrev[SDL_SCANCODE_RIGHTBRACKET])
            held = true;
    }
    else if(arg == key::MINUS){
        if(controls.keyboard[SDL_SCANCODE_MINUS] && controls.keyboardFramePrev[SDL_SCANCODE_MINUS])
            held = true;
    }
    else if(arg == key::RALT){
        if(controls.keyboard[SDL_SCANCODE_RALT] && controls.keyboardFramePrev[SDL_SCANCODE_RALT])
            held = true;
    }
    else if(arg == key::RCTRL){
        if(controls.keyboard[SDL_SCANCODE_RCTRL] && controls.keyboardFramePrev[SDL_SCANCODE_RCTRL])
            held = true;
    }
    else if(arg == key::SEMICOLON){
        if(controls.keyboard[SDL_SCANCODE_SEMICOLON] && controls.keyboardFramePrev[SDL_SCANCODE_SEMICOLON])
            held = true;
    }
    else if(arg == key::SLASH){
        if(controls.keyboard[SDL_SCANCODE_SLASH] && controls.keyboardFramePrev[SDL_SCANCODE_SLASH])
            held = true;
    }
    else if(arg == key::TAB){
        if(controls.keyboard[SDL_SCANCODE_TAB] && controls.keyboardFramePrev[SDL_SCANCODE_TAB])
            held = true;
    }
    else if(arg == key::MOUSELEFT){
        if(controls.mouseClicksFramePrev[0] && controls.mouseClicks[0])
            held = true;
    }
    else if(arg == key::MOUSEMIDDLE){
        if(controls.mouseClicksFramePrev[1] && controls.mouseClicks[1])
            held = true;
    }
    else if(arg == key::MOUSERIGHT){
        if(controls.mouseClicksFramePrev[2] && controls.mouseClicks[2])
            held = true;
    }
    return held;
}

bool input_manager::keyrelease(key arg){
    bool released = false;
    if(arg == key::LEFT){
        if(!controls.keyboard[SDL_SCANCODE_LEFT] && controls.keyboardFramePrev[SDL_SCANCODE_LEFT])
            released = true;
    }
    else if(arg == key::RIGHT){
        if(!controls.keyboard[SDL_SCANCODE_RIGHT] && controls.keyboardFramePrev[SDL_SCANCODE_RIGHT])
            released = true;
    }
    else if(arg == key::UP){
        if(!controls.keyboard[SDL_SCANCODE_UP] && controls.keyboardFramePrev[SDL_SCANCODE_UP])
            released = true;
    }
    else if(arg == key::DOWN){
        if(!controls.keyboard[SDL_SCANCODE_DOWN] && controls.keyboardFramePrev[SDL_SCANCODE_DOWN])
            released = true;
    }
    else if(arg == key::A){
        if(!controls.keyboard[SDL_SCANCODE_A] && controls.keyboardFramePrev[SDL_SCANCODE_A])
            released = true;
    }
    else if(arg == key::B){
        if(!controls.keyboard[SDL_SCANCODE_B] && controls.keyboardFramePrev[SDL_SCANCODE_B])
            released = true;
    }
    else if(arg == key::C){
        if(!controls.keyboard[SDL_SCANCODE_C] && controls.keyboardFramePrev[SDL_SCANCODE_C])
            released = true;
    }
    else if(arg == key::D){
        if(!controls.keyboard[SDL_SCANCODE_D] && controls.keyboardFramePrev[SDL_SCANCODE_D])
            released = true;
    }
    else if(arg == key::E){
        if(!controls.keyboard[SDL_SCANCODE_E] && controls.keyboardFramePrev[SDL_SCANCODE_E])
            released = true;
    }
    else if(arg == key::F){
        if(!controls.keyboard[SDL_SCANCODE_F] && controls.keyboardFramePrev[SDL_SCANCODE_F])
            released = true;
    }
    else if(arg == key::G){
        if(!controls.keyboard[SDL_SCANCODE_G] && controls.keyboardFramePrev[SDL_SCANCODE_G])
            released = true;
    }
    else if(arg == key::H){
        if(!controls.keyboard[SDL_SCANCODE_H] && controls.keyboardFramePrev[SDL_SCANCODE_H])
            released = true;
    }
    else if(arg == key::I){
        if(!controls.keyboard[SDL_SCANCODE_I] && controls.keyboardFramePrev[SDL_SCANCODE_I])
            released = true;
    }
    else if(arg == key::J){
        if(!controls.keyboard[SDL_SCANCODE_J] && controls.keyboardFramePrev[SDL_SCANCODE_G])
            released = true;
    }
    else if(arg == key::K){
        if(!controls.keyboard[SDL_SCANCODE_K] && controls.keyboardFramePrev[SDL_SCANCODE_K])
            released = true;
    }
    else if(arg == key::L){
        if(!controls.keyboard[SDL_SCANCODE_L] && controls.keyboardFramePrev[SDL_SCANCODE_L])
            released = true;
    }
    else if(arg == key::M){
        if(!controls.keyboard[SDL_SCANCODE_M] && controls.keyboardFramePrev[SDL_SCANCODE_M])
            released = true;
    }
    else if(arg == key::N){
        if(!controls.keyboard[SDL_SCANCODE_N] && controls.keyboardFramePrev[SDL_SCANCODE_N])
            released = true;
    }
    else if(arg == key::O){
        if(!controls.keyboard[SDL_SCANCODE_O] && controls.keyboardFramePrev[SDL_SCANCODE_O])
            released = true;
    }
    else if(arg == key::P){
        if(!controls.keyboard[SDL_SCANCODE_P] && controls.keyboardFramePrev[SDL_SCANCODE_P])
            released = true;
    }
    else if(arg == key::Q){
        if(!controls.keyboard[SDL_SCANCODE_Q] && controls.keyboardFramePrev[SDL_SCANCODE_Q])
            released = true;
    }
    else if(arg == key::R){
        if(!controls.keyboard[SDL_SCANCODE_R] && controls.keyboardFramePrev[SDL_SCANCODE_R])
            released = true;
    }
    else if(arg == key::S){
        if(!controls.keyboard[SDL_SCANCODE_S] && controls.keyboardFramePrev[SDL_SCANCODE_S])
            released = true;
    }
    else if(arg == key::T){
        if(!controls.keyboard[SDL_SCANCODE_T] && controls.keyboardFramePrev[SDL_SCANCODE_T])
            released = true;
    }
    else if(arg == key::U){
        if(!controls.keyboard[SDL_SCANCODE_U] && controls.keyboardFramePrev[SDL_SCANCODE_U])
            released = true;
    }
    else if(arg == key::V){
        if(!controls.keyboard[SDL_SCANCODE_V] && controls.keyboardFramePrev[SDL_SCANCODE_V])
            released = true;
    }
    else if(arg == key::W){
        if(!controls.keyboard[SDL_SCANCODE_W] && controls.keyboardFramePrev[SDL_SCANCODE_W])
            released = true;
    }
    else if(arg == key::X){
        if(!controls.keyboard[SDL_SCANCODE_X] && controls.keyboardFramePrev[SDL_SCANCODE_X])
            released = true;
    }
    else if(arg == key::Y){
        if(!controls.keyboard[SDL_SCANCODE_Y] && controls.keyboardFramePrev[SDL_SCANCODE_Y])
            released = true;
    }
    else if(arg == key::Z){
        if(!controls.keyboard[SDL_SCANCODE_Z] && controls.keyboardFramePrev[SDL_SCANCODE_Z])
            released = true;
    }
    else if(arg == key::ONE){
        if(!controls.keyboard[SDL_SCANCODE_1] && controls.keyboardFramePrev[SDL_SCANCODE_1])
            released = true;
    }
    else if(arg == key::TWO){
        if(!controls.keyboard[SDL_SCANCODE_2] && controls.keyboardFramePrev[SDL_SCANCODE_2])
            released = true;
    }
    else if(arg == key::THREE){
        if(!controls.keyboard[SDL_SCANCODE_3] && controls.keyboardFramePrev[SDL_SCANCODE_3])
            released = true;
    }
    else if(arg == key::FOUR){
        if(!controls.keyboard[SDL_SCANCODE_4] && controls.keyboardFramePrev[SDL_SCANCODE_4])
            released = true;
    }
    else if(arg == key::FIVE){
        if(!controls.keyboard[SDL_SCANCODE_5] && controls.keyboardFramePrev[SDL_SCANCODE_5])
            released = true;
    }
    else if(arg == key::SIX){
        if(!controls.keyboard[SDL_SCANCODE_6] && controls.keyboardFramePrev[SDL_SCANCODE_6])
            released = true;
    }
    else if(arg == key::SEVEN){
        if(!controls.keyboard[SDL_SCANCODE_7] && controls.keyboardFramePrev[SDL_SCANCODE_7])
            released = true;
    }
    else if(arg == key::EIGHT){
        if(!controls.keyboard[SDL_SCANCODE_8] && controls.keyboardFramePrev[SDL_SCANCODE_8])
            released = true;
    }
    else if(arg == key::NINE){
        if(!controls.keyboard[SDL_SCANCODE_9] && controls.keyboardFramePrev[SDL_SCANCODE_9])
            released = true;
    }
    else if(arg == key::ZERO){
        if(!controls.keyboard[SDL_SCANCODE_0] && controls.keyboardFramePrev[SDL_SCANCODE_0])
            released = true;
    }
    else if(arg == key::ESCAPE){
        if(!controls.keyboard[SDL_SCANCODE_ESCAPE] && controls.keyboardFramePrev[SDL_SCANCODE_ESCAPE])
            released = true;
    }
    else if(arg == key::ENTER){
        if(!controls.keyboard[SDL_SCANCODE_RETURN] && controls.keyboardFramePrev[SDL_SCANCODE_RETURN])
            released = true;
    }
    else if(arg == key::SPACEBAR){
        if(!controls.keyboard[SDL_SCANCODE_SPACE] && controls.keyboardFramePrev[SDL_SCANCODE_SPACE])
            released = true;
    }
    else if(arg == key::TILDE){
        if(!controls.keyboard[SDL_SCANCODE_GRAVE] && controls.keyboardFramePrev[SDL_SCANCODE_GRAVE])
            released = true;
    }
    else if(arg == key::LSHIFT){
        if(!controls.keyboard[SDL_SCANCODE_LSHIFT] && controls.keyboardFramePrev[SDL_SCANCODE_LSHIFT])
            released = true;
    }
    else if(arg == key::BACKSPACE){
        if(!controls.keyboard[SDL_SCANCODE_BACKSPACE] && controls.keyboardFramePrev[SDL_SCANCODE_BACKSPACE])
            released = true;
    }
    else if(arg == key::BACKSLASH){
        if(!controls.keyboard[SDL_SCANCODE_BACKSLASH] && controls.keyboardFramePrev[SDL_SCANCODE_BACKSLASH])
            released = true;
    }
    else if(arg == key::CAPSLOCK){
        if(!controls.keyboard[SDL_SCANCODE_CAPSLOCK] && controls.keyboardFramePrev[SDL_SCANCODE_CAPSLOCK])
            released = true;
    }
    else if(arg == key::COMMA){
        if(!controls.keyboard[SDL_SCANCODE_COMMA] && controls.keyboardFramePrev[SDL_SCANCODE_COMMA])
            released = true;
    }
    else if(arg == key::EQUALS){
        if(!controls.keyboard[SDL_SCANCODE_EQUALS] && controls.keyboardFramePrev[SDL_SCANCODE_EQUALS])
            released = true;
    }
    else if(arg == key::LALT){
        if(!controls.keyboard[SDL_SCANCODE_LALT] && controls.keyboardFramePrev[SDL_SCANCODE_LALT])
            released = true;
    }
    else if(arg == key::LCTRL){
        if(!controls.keyboard[SDL_SCANCODE_LCTRL] && controls.keyboardFramePrev[SDL_SCANCODE_LCTRL])
            released = true;
    }
    else if(arg == key::LBRACKET){
        if(!controls.keyboard[SDL_SCANCODE_LEFTBRACKET] && controls.keyboardFramePrev[SDL_SCANCODE_LEFTBRACKET])
            released = true;
    }
    else if(arg == key::RBRACKET){
        if(!controls.keyboard[SDL_SCANCODE_RIGHTBRACKET] && controls.keyboardFramePrev[SDL_SCANCODE_RIGHTBRACKET])
            released = true;
    }
    else if(arg == key::MINUS){
        if(!controls.keyboard[SDL_SCANCODE_MINUS] && controls.keyboardFramePrev[SDL_SCANCODE_MINUS])
            released = true;
    }
    else if(arg == key::RALT){
        if(!controls.keyboard[SDL_SCANCODE_RALT] && controls.keyboardFramePrev[SDL_SCANCODE_RALT])
            released = true;
    }
    else if(arg == key::RCTRL){
        if(!controls.keyboard[SDL_SCANCODE_RCTRL] && controls.keyboardFramePrev[SDL_SCANCODE_RCTRL])
            released = true;
    }
    else if(arg == key::SEMICOLON){
        if(!controls.keyboard[SDL_SCANCODE_SEMICOLON] && controls.keyboardFramePrev[SDL_SCANCODE_SEMICOLON])
            released = true;
    }
    else if(arg == key::SLASH){
        if(!controls.keyboard[SDL_SCANCODE_SLASH] && controls.keyboardFramePrev[SDL_SCANCODE_SLASH])
            released = true;
    }
    else if(arg == key::TAB){
        if(!controls.keyboard[SDL_SCANCODE_TAB] && controls.keyboardFramePrev[SDL_SCANCODE_TAB])
            released = true;
    }
    else if(arg == key::MOUSELEFT){
        if(controls.mouseClicksFramePrev[0] && !controls.mouseClicks[0])
            released = true;
    }
    else if(arg == key::MOUSEMIDDLE){
        if(controls.mouseClicksFramePrev[1] && !controls.mouseClicks[1])
            released = true;
    }
    else if(arg == key::MOUSERIGHT){
        if(controls.mouseClicksFramePrev[2] && !controls.mouseClicks[2])
            released = true;
    }
    return released;
}

void input_manager::close(){
    free(controls.keyboardFramePrev);
    delete msg_sub;
}
