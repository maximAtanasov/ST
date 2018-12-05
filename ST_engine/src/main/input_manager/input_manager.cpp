/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <input_manager/input_manager.hpp>
#include <console/log.hpp>

/**
 * initializes the input manager
 * @param msg_bus A pointer to the global message bus.
 * @param tsk_mngr A pointer to the global task_mngr.
 */
input_manager::input_manager(message_bus* msg_bus, task_manager* tsk_mngr){

    //SET OUR EXTERNAL DEPENDENCIES
    gMessage_bus = msg_bus;
    gTask_manager = tsk_mngr;

    if( SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) < 0 ){
        log(ERROR, "Could not initialize gamepad subsystem!");
    }

    //Initialize controls
    int32_t length = 0;
    controls.keyboard = SDL_GetKeyboardState(&length);
    SDL_PollEvent(&event);
    controls.mouseX = 0;
    controls.mouseY = 0;
    controls.mouseX_prev = controls.mouseX;
    controls.mouseY_prev = controls.mouseY;
    controls.mouse_scroll = 0;

    //Subscribe to the messages we need
	gMessage_bus->subscribe(VIRTUAL_SCREEN_COORDINATES, &msg_sub);
	gMessage_bus->subscribe(REAL_SCREEN_COORDINATES, &msg_sub);
    gMessage_bus->subscribe(START_TEXT_INPUT, &msg_sub);
    gMessage_bus->subscribe(STOP_TEXT_INPUT, &msg_sub);
    gMessage_bus->subscribe(CLEAR_TEXT_STREAM, &msg_sub);
    gMessage_bus->subscribe(REGISTER_KEY, &msg_sub);
    gMessage_bus->subscribe(UNREGISTER_KEY, &msg_sub);
}

/**
 * Performs the update for the input_manager on a task thread.
 * @param arg pointer to an input_manager (a <b>this</b> pointer basically) as the
 * function must be static.
 */
void input_manager::update_task(void* mngr){
    auto self = static_cast<input_manager*>(mngr);
    self->handle_messages();
    self->take_input();
}

/**
 * Checks the state of the keyboard/mouse and sends appropriate messages.
 */
void input_manager::take_input(){
    int length = 0;
    SDL_GetKeyboardState(&length);
    for(uint16_t i = 0; i < controls.keys; i++){
        controls.keyboardFramePrev[i] = controls.keyboard[i];
    }
    controller_button_prev_frame = controller_buttons;

    //Collect mouse input
	controls.mouseClicksFramePrev[0] = controls.mouseClicks[0];
	controls.mouseClicksFramePrev[1] = controls.mouseClicks[1];
	controls.mouseClicksFramePrev[2] = controls.mouseClicks[2];
    for(int8_t& i : controls.mouseClicks){
        i = 0;
    }
    if(SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        controls.mouseClicks[0] = 1;
    }
    if(SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_MIDDLE)) {
        controls.mouseClicks[1] = 1;
    }
    if(SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
        controls.mouseClicks[2] = 1;
    }

    controller_buttons.a = 0;
    controller_buttons.b = 0;
    controller_buttons.x = 0;
    controller_buttons.y = 0;
    controller_buttons.dpad_up = 0;
    controller_buttons.dpad_right = 0;
    controller_buttons.dpad_down = 0;
    controller_buttons.dpad_left = 0;
    controller_buttons.left_stick = 0;
    controller_buttons.right_stick = 0;
    controller_buttons.left_shoulder = 0;
    controller_buttons.right_shoulder = 0;
    controller_buttons.select = 0;
    controller_buttons.start = 0;
    for(SDL_GameController* c : controllers) {
        if (SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_A)){
            controller_buttons.a = 1;
        }
        if (SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_B)){
            controller_buttons.b = 1;
        }
        if (SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_X)){
            controller_buttons.x = 1;
        }
        if (SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_Y)){
            controller_buttons.y = 1;
        }
        if (SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_DPAD_UP)){
            controller_buttons.dpad_up = 1;
        }
        if (SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_DPAD_DOWN)){
            controller_buttons.dpad_down = 1;
        }
        if (SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_DPAD_LEFT)){
            controller_buttons.dpad_left = 1;
        }
        if (SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_DPAD_RIGHT)){
            controller_buttons.dpad_right = 1;
        }
        if (SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_LEFTSTICK)){
            controller_buttons.left_stick = 1;
        }
        if (SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_RIGHTSTICK)){
            controller_buttons.right_stick = 1;
        }
        if (SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_LEFTSHOULDER)){
            controller_buttons.left_shoulder = 1;
        }
        if (SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)){
            controller_buttons.right_shoulder = 1;
        }
        if (SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_START)){
            controller_buttons.start = 1;
        }
        if (SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_BACK)){
            controller_buttons.select = 1;
        }
    }

    while(SDL_PollEvent(&event) != 0){
        if(event.type == SDL_QUIT){
            gMessage_bus->send_msg(make_msg(END_GAME, nullptr));
        }
        if(event.type == SDL_MOUSEWHEEL){
            controls.mouse_scroll += event.wheel.y;
            if(controls.mouse_scroll < 0){
                controls.mouse_scroll = 0;
            }
            gMessage_bus->send_msg(make_msg(MOUSE_SCROLL, make_data(controls.mouse_scroll)));
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
                gMessage_bus->send_msg(make_msg(TEXT_STREAM, make_data(composition)));
            }
        }
        if(event.type == SDL_KEYDOWN){
            if(event.key.keysym.sym == SDLK_BACKSPACE) {
                if (composition.length() > 0) {
                    composition.pop_back();
                    gMessage_bus->send_msg(make_msg(TEXT_STREAM, make_data(composition)));
                }
            }
        }
		if(event.type == SDL_MOUSEMOTION){
			SDL_GetMouseState(&controls.mouseX, &controls.mouseY);
			controls.mouseX = static_cast<int>(static_cast<float>(controls.mouseX)*ratio_w);
			controls.mouseY = static_cast<int>(static_cast<float>(controls.mouseY)*ratio_h);
		}
        if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED){
            r_width = event.window.data1;
            r_height = event.window.data2;
            ratio_w = static_cast<float>(v_width) / static_cast<float>(r_width);
            ratio_h = static_cast<float>(v_height) / static_cast<float>(r_height);
        }
        if(event.cdevice.type == SDL_CONTROLLERDEVICEADDED){
            SDL_GameController* controller = SDL_GameControllerOpen(static_cast<int>(controllers.size()));
            controllers.emplace_back(controller);
            log(INFO, "Found a valid controller: " + std::string(SDL_GameControllerName(controller)));
        }
        if(event.cdevice.type == SDL_CONTROLLERDEVICEREMOVED){
            uint8_t number = 0;
            for(uint8_t i = 0; i < controllers.size(); i++){
                if(SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controllers.at(i))) == event.cdevice.which){
                    number = i;
                }
            }
            controllers.erase(controllers.begin() + number);
            log(INFO, "Controller " + std::to_string(number+1) + " removed");
        }
    }

    //check if any of the registered keys is pressed and send a message if so
    for(auto i : registered_keys){
        if(i.second > 0) {
            if (keypress(i.first)) {
                gMessage_bus->send_msg(make_msg(KEY_PRESSED, make_data(static_cast<uint8_t>(i.first))));
            } else if (keyheld(i.first)) {
                gMessage_bus->send_msg(make_msg(KEY_HELD, make_data(static_cast<uint8_t>(i.first))));
            } else if (keyrelease(i.first)) {
                gMessage_bus->send_msg(make_msg(KEY_RELEASED, make_data(static_cast<uint8_t>(i.first))));
            }
        }
    }

    //only send mouse coordinates if they change
    if(controls.mouseX != controls.mouseX_prev){
        gMessage_bus->send_msg(make_msg(MOUSE_X, make_data(controls.mouseX)));
        controls.mouseX_prev = controls.mouseX;
    }
    if(controls.mouseY != controls.mouseY_prev){
        gMessage_bus->send_msg(make_msg(MOUSE_Y, make_data(controls.mouseY)));
        controls.mouseY_prev = controls.mouseY;
    }
}

/**
 * Retrieves messages from the subscriber object and
 * performs the appropriate actions.
 */
void input_manager::handle_messages(){
	message* temp = msg_sub.get_next_message();
	while(temp != nullptr){
		if(temp->msg_name == VIRTUAL_SCREEN_COORDINATES){
            auto data = static_cast<std::tuple<int16_t, int16_t>*>(temp->get_data());
            v_width = std::get<0> (*data);
            v_height = std::get<1> (*data);
            ratio_w = static_cast<float>(v_width) / static_cast<float>(r_width);
            ratio_h = static_cast<float>(v_height) / static_cast<float>(r_height);
		}else if(temp->msg_name == REAL_SCREEN_COORDINATES) {
            auto data = static_cast<std::tuple<int16_t, int16_t>*>(temp->get_data());
            r_width = std::get<0>(*data);
            r_height = std::get<1>(*data);
            ratio_w = static_cast<float>(v_width) / static_cast<float>(r_width);
            ratio_h = static_cast<float>(v_height) / static_cast<float>(r_height);
        }else if(temp->msg_name == START_TEXT_INPUT){
            text_input = true;
        }else if(temp->msg_name == STOP_TEXT_INPUT){
            text_input = false;
        }else if(temp->msg_name == CLEAR_TEXT_STREAM){
            composition.clear();
            gMessage_bus->send_msg(make_msg(TEXT_STREAM, make_data(composition)));
        }else if(temp->msg_name == REGISTER_KEY){
            auto key_val = static_cast<ST::key*>(temp->get_data());
            ++registered_keys[*key_val];
        }else if(temp->msg_name == UNREGISTER_KEY){
            auto key_val = static_cast<ST::key*>(temp->get_data());
            if(registered_keys[*key_val] > 0) {
                --registered_keys[*key_val];
            }
        }
		destroy_msg(temp);
		temp = msg_sub.get_next_message();
	}
}

/**
 * Tells if a key is pressed or not. Works by comparing it's
 * current state with the state from the previous frame.
 * @param arg an <b>ST::key</b> to key for being pressed.
 * @return True if pressed, false otherwise.
 */
bool input_manager::keypress(ST::key arg) const{
    bool pressed = false;
    if(arg == ST::key::LEFT){
        if(controls.keyboard[SDL_SCANCODE_LEFT] && !controls.keyboardFramePrev[SDL_SCANCODE_LEFT])
            pressed = true;
    }
    else if(arg == ST::key::RIGHT){
        if(controls.keyboard[SDL_SCANCODE_RIGHT] && !controls.keyboardFramePrev[SDL_SCANCODE_RIGHT])
            pressed = true;
    }
    else if(arg == ST::key::UP){
        if(controls.keyboard[SDL_SCANCODE_UP] && !controls.keyboardFramePrev[SDL_SCANCODE_UP])
            pressed = true;
    }
    else if(arg == ST::key::DOWN){
        if(controls.keyboard[SDL_SCANCODE_DOWN] && !controls.keyboardFramePrev[SDL_SCANCODE_DOWN])
            pressed = true;
    }
    else if(arg == ST::key::A){
        if(controls.keyboard[SDL_SCANCODE_A] && !controls.keyboardFramePrev[SDL_SCANCODE_A])
            pressed = true;
    }
    else if(arg == ST::key::B){
        if(controls.keyboard[SDL_SCANCODE_B] && !controls.keyboardFramePrev[SDL_SCANCODE_B])
            pressed = true;
    }
    else if(arg == ST::key::C){
        if(controls.keyboard[SDL_SCANCODE_C] && !controls.keyboardFramePrev[SDL_SCANCODE_C])
            pressed = true;
    }
    else if(arg == ST::key::D){
        if(controls.keyboard[SDL_SCANCODE_D] && !controls.keyboardFramePrev[SDL_SCANCODE_D])
            pressed = true;
    }
    else if(arg == ST::key::E){
        if(controls.keyboard[SDL_SCANCODE_E] && !controls.keyboardFramePrev[SDL_SCANCODE_E])
            pressed = true;
    }
    else if(arg == ST::key::F){
        if(controls.keyboard[SDL_SCANCODE_F] && !controls.keyboardFramePrev[SDL_SCANCODE_F])
            pressed = true;
    }
    else if(arg == ST::key::G){
        if(controls.keyboard[SDL_SCANCODE_G] && !controls.keyboardFramePrev[SDL_SCANCODE_G])
            pressed = true;
    }
    else if(arg == ST::key::H){
        if(controls.keyboard[SDL_SCANCODE_H] && !controls.keyboardFramePrev[SDL_SCANCODE_H])
            pressed = true;
    }
    else if(arg == ST::key::I){
        if(controls.keyboard[SDL_SCANCODE_I] && !controls.keyboardFramePrev[SDL_SCANCODE_I])
            pressed = true;
    }
    else if(arg == ST::key::J){
        if(controls.keyboard[SDL_SCANCODE_J] && !controls.keyboardFramePrev[SDL_SCANCODE_G])
            pressed = true;
    }
    else if(arg == ST::key::K){
        if(controls.keyboard[SDL_SCANCODE_K] && !controls.keyboardFramePrev[SDL_SCANCODE_K])
            pressed = true;
    }
    else if(arg == ST::key::L){
        if(controls.keyboard[SDL_SCANCODE_L] && !controls.keyboardFramePrev[SDL_SCANCODE_L])
            pressed = true;
    }
    else if(arg == ST::key::M){
        if(controls.keyboard[SDL_SCANCODE_M] && !controls.keyboardFramePrev[SDL_SCANCODE_M])
            pressed = true;
    }
    else if(arg == ST::key::N){
        if(controls.keyboard[SDL_SCANCODE_N] && !controls.keyboardFramePrev[SDL_SCANCODE_N])
            pressed = true;
    }
    else if(arg == ST::key::O){
        if(controls.keyboard[SDL_SCANCODE_O] && !controls.keyboardFramePrev[SDL_SCANCODE_O])
            pressed = true;
    }
    else if(arg == ST::key::P){
        if(controls.keyboard[SDL_SCANCODE_P] && !controls.keyboardFramePrev[SDL_SCANCODE_P])
            pressed = true;
    }
    else if(arg == ST::key::Q){
        if(controls.keyboard[SDL_SCANCODE_Q] && !controls.keyboardFramePrev[SDL_SCANCODE_Q])
            pressed = true;
    }
    else if(arg == ST::key::R){
        if(controls.keyboard[SDL_SCANCODE_R] && !controls.keyboardFramePrev[SDL_SCANCODE_R])
            pressed = true;
    }
    else if(arg == ST::key::S){
        if(controls.keyboard[SDL_SCANCODE_S] && !controls.keyboardFramePrev[SDL_SCANCODE_S])
            pressed = true;
    }
    else if(arg == ST::key::T){
        if(controls.keyboard[SDL_SCANCODE_T] && !controls.keyboardFramePrev[SDL_SCANCODE_T])
            pressed = true;
    }
    else if(arg == ST::key::U){
        if(controls.keyboard[SDL_SCANCODE_U] && !controls.keyboardFramePrev[SDL_SCANCODE_U])
            pressed = true;
    }
    else if(arg == ST::key::V){
        if(controls.keyboard[SDL_SCANCODE_V] && !controls.keyboardFramePrev[SDL_SCANCODE_V])
            pressed = true;
    }
    else if(arg == ST::key::W){
        if(controls.keyboard[SDL_SCANCODE_W] && !controls.keyboardFramePrev[SDL_SCANCODE_W])
            pressed = true;
    }
    else if(arg == ST::key::X){
        if(controls.keyboard[SDL_SCANCODE_X] && !controls.keyboardFramePrev[SDL_SCANCODE_X])
            pressed = true;
    }
    else if(arg == ST::key::Y){
        if(controls.keyboard[SDL_SCANCODE_Y] && !controls.keyboardFramePrev[SDL_SCANCODE_Y])
            pressed = true;
    }
    else if(arg == ST::key::Z){
        if(controls.keyboard[SDL_SCANCODE_Z] && !controls.keyboardFramePrev[SDL_SCANCODE_Z])
            pressed = true;
    }
    else if(arg == ST::key::ONE){
        if(controls.keyboard[SDL_SCANCODE_1] && !controls.keyboardFramePrev[SDL_SCANCODE_1])
            pressed = true;
    }
    else if(arg == ST::key::TWO){
        if(controls.keyboard[SDL_SCANCODE_2] && !controls.keyboardFramePrev[SDL_SCANCODE_2])
            pressed = true;
    }
    else if(arg == ST::key::THREE){
        if(controls.keyboard[SDL_SCANCODE_3] && !controls.keyboardFramePrev[SDL_SCANCODE_3])
            pressed = true;
    }
    else if(arg == ST::key::FOUR){
        if(controls.keyboard[SDL_SCANCODE_4] && !controls.keyboardFramePrev[SDL_SCANCODE_4])
            pressed = true;
    }
    else if(arg == ST::key::FIVE){
        if(controls.keyboard[SDL_SCANCODE_5] && !controls.keyboardFramePrev[SDL_SCANCODE_5])
            pressed = true;
    }
    else if(arg == ST::key::SIX){
        if(controls.keyboard[SDL_SCANCODE_6] && !controls.keyboardFramePrev[SDL_SCANCODE_6])
            pressed = true;
    }
    else if(arg == ST::key::SEVEN){
        if(controls.keyboard[SDL_SCANCODE_7] && !controls.keyboardFramePrev[SDL_SCANCODE_7])
            pressed = true;
    }
    else if(arg == ST::key::EIGHT){
        if(controls.keyboard[SDL_SCANCODE_8] && !controls.keyboardFramePrev[SDL_SCANCODE_8])
            pressed = true;
    }
    else if(arg == ST::key::NINE){
        if(controls.keyboard[SDL_SCANCODE_9] && !controls.keyboardFramePrev[SDL_SCANCODE_9])
            pressed = true;
    }
    else if(arg == ST::key::ZERO){
        if(controls.keyboard[SDL_SCANCODE_0] && !controls.keyboardFramePrev[SDL_SCANCODE_0])
            pressed = true;
    }
    else if(arg == ST::key::ESCAPE){
        if(controls.keyboard[SDL_SCANCODE_ESCAPE] && !controls.keyboardFramePrev[SDL_SCANCODE_ESCAPE])
            pressed = true;
    }
    else if(arg == ST::key::ENTER){
        if(controls.keyboard[SDL_SCANCODE_RETURN] && !controls.keyboardFramePrev[SDL_SCANCODE_RETURN])
            pressed = true;
    }
    else if(arg == ST::key::SPACEBAR){
        if(controls.keyboard[SDL_SCANCODE_SPACE] && !controls.keyboardFramePrev[SDL_SCANCODE_SPACE])
            pressed = true;
    }
    else if(arg == ST::key::TILDE){
        if(controls.keyboard[SDL_SCANCODE_GRAVE] && !controls.keyboardFramePrev[SDL_SCANCODE_GRAVE])
            pressed = true;
    }
    else if(arg == ST::key::LSHIFT){
        if(controls.keyboard[SDL_SCANCODE_LSHIFT] && !controls.keyboardFramePrev[SDL_SCANCODE_LSHIFT])
            pressed = true;
    }
    else if(arg == ST::key::BACKSPACE){
        if(controls.keyboard[SDL_SCANCODE_BACKSPACE] && !controls.keyboardFramePrev[SDL_SCANCODE_BACKSPACE])
            pressed = true;
    }
    else if(arg == ST::key::BACKSLASH){
        if(controls.keyboard[SDL_SCANCODE_BACKSLASH] && !controls.keyboardFramePrev[SDL_SCANCODE_BACKSLASH])
            pressed = true;
    }
    else if(arg == ST::key::CAPSLOCK){
        if(controls.keyboard[SDL_SCANCODE_CAPSLOCK] && !controls.keyboardFramePrev[SDL_SCANCODE_CAPSLOCK])
            pressed = true;
    }
    else if(arg == ST::key::COMMA){
        if(controls.keyboard[SDL_SCANCODE_COMMA] && !controls.keyboardFramePrev[SDL_SCANCODE_COMMA])
            pressed = true;
    }
    else if(arg == ST::key::EQUALS){
        if(controls.keyboard[SDL_SCANCODE_EQUALS] && !controls.keyboardFramePrev[SDL_SCANCODE_EQUALS])
            pressed = true;
    }
    else if(arg == ST::key::LALT){
        if(controls.keyboard[SDL_SCANCODE_LALT] && !controls.keyboardFramePrev[SDL_SCANCODE_LALT])
            pressed = true;
    }
    else if(arg == ST::key::LCTRL){
        if(controls.keyboard[SDL_SCANCODE_LCTRL] && !controls.keyboardFramePrev[SDL_SCANCODE_LCTRL])
            pressed = true;
    }
    else if(arg == ST::key::LBRACKET){
        if(controls.keyboard[SDL_SCANCODE_LEFTBRACKET] && !controls.keyboardFramePrev[SDL_SCANCODE_LEFTBRACKET])
            pressed = true;
    }
    else if(arg == ST::key::RBRACKET){
        if(controls.keyboard[SDL_SCANCODE_RIGHTBRACKET] && !controls.keyboardFramePrev[SDL_SCANCODE_RIGHTBRACKET])
            pressed = true;
    }
    else if(arg == ST::key::MINUS){
        if(controls.keyboard[SDL_SCANCODE_MINUS] && !controls.keyboardFramePrev[SDL_SCANCODE_MINUS])
            pressed = true;
    }
    else if(arg == ST::key::RALT){
        if(controls.keyboard[SDL_SCANCODE_RALT] && !controls.keyboardFramePrev[SDL_SCANCODE_RALT])
            pressed = true;
    }
    else if(arg == ST::key::RCTRL){
        if(controls.keyboard[SDL_SCANCODE_RCTRL] && !controls.keyboardFramePrev[SDL_SCANCODE_RCTRL])
            pressed = true;
    }
    else if(arg == ST::key::SEMICOLON){
        if(controls.keyboard[SDL_SCANCODE_SEMICOLON] && !controls.keyboardFramePrev[SDL_SCANCODE_SEMICOLON])
            pressed = true;
    }
    else if(arg == ST::key::SLASH){
        if(controls.keyboard[SDL_SCANCODE_SLASH] && !controls.keyboardFramePrev[SDL_SCANCODE_SLASH])
            pressed = true;
    }
    else if(arg == ST::key::TAB){
        if(controls.keyboard[SDL_SCANCODE_TAB] && !controls.keyboardFramePrev[SDL_SCANCODE_TAB])
            pressed = true;
    }
    else if(arg == ST::key::MOUSELEFT){
        if(!controls.mouseClicksFramePrev[0] && controls.mouseClicks[0])
            pressed = true;
    }
    else if(arg == ST::key::MOUSEMIDDLE){
        if(!controls.mouseClicksFramePrev[1] && controls.mouseClicks[1])
            pressed = true;
    }
    else if(arg == ST::key::MOUSERIGHT){
        if(!controls.mouseClicksFramePrev[2] && controls.mouseClicks[2])
            pressed = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_A){
        if(!controller_button_prev_frame.a && controller_buttons.a)
            pressed = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_B){
        if(!controller_button_prev_frame.b && controller_buttons.b)
            pressed = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_X){
        if(!controller_button_prev_frame.x && controller_buttons.x)
            pressed = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_Y){
        if(!controller_button_prev_frame.y && controller_buttons.y)
            pressed = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_LEFTSTICK){
        if(!controller_button_prev_frame.left_stick && controller_buttons.left_stick)
            pressed = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_RIGHTSTICK){
        if(!controller_button_prev_frame.right_stick && controller_buttons.right_stick)
            pressed = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_DPAD_UP){
        if(!controller_button_prev_frame.dpad_up && controller_buttons.dpad_up)
            pressed = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_DPAD_DOWN){
        if(!controller_button_prev_frame.dpad_down && controller_buttons.dpad_down)
            pressed = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_DPAD_LEFT){
        if(!controller_button_prev_frame.dpad_left && controller_buttons.dpad_left)
            pressed = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_DPAD_RIGHT){
        if(!controller_button_prev_frame.dpad_right && controller_buttons.dpad_right)
            pressed = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_LEFTSHOULDER){
        if(!controller_button_prev_frame.left_shoulder && controller_buttons.left_shoulder)
            pressed = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_RIGHTSHOULDER){
        if(!controller_button_prev_frame.right_shoulder && controller_buttons.right_shoulder)
            pressed = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_SELECT){
        if(!controller_button_prev_frame.select && controller_buttons.select)
            pressed = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_START){
        if(!controller_button_prev_frame.start && controller_buttons.start)
            pressed = true;
    }
    return pressed;
}

/**
 * Tells if a key is held or not. Works by comparing it's
 * current state with the state from the previous frame.
 * @param arg an <b>ST::key</b> to key for being held.
 * @return True if held, false otherwise.
 */
bool input_manager::keyheld(ST::key arg) const{
    bool held = false;
    if(arg == ST::key::LEFT){
        if(controls.keyboard[SDL_SCANCODE_LEFT] && controls.keyboardFramePrev[SDL_SCANCODE_LEFT])
            held = true;
    }
    else if(arg == ST::key::RIGHT){
        if(controls.keyboard[SDL_SCANCODE_RIGHT] && controls.keyboardFramePrev[SDL_SCANCODE_RIGHT])
            held = true;
    }
    else if(arg == ST::key::UP){
        if(controls.keyboard[SDL_SCANCODE_UP] && controls.keyboardFramePrev[SDL_SCANCODE_UP])
            held = true;
    }
    else if(arg == ST::key::DOWN){
        if(controls.keyboard[SDL_SCANCODE_DOWN] && controls.keyboardFramePrev[SDL_SCANCODE_DOWN])
            held = true;
    }
    else if(arg == ST::key::A){
        if(controls.keyboard[SDL_SCANCODE_A] && controls.keyboardFramePrev[SDL_SCANCODE_A])
            held = true;
    }
    else if(arg == ST::key::B){
        if(controls.keyboard[SDL_SCANCODE_B] && controls.keyboardFramePrev[SDL_SCANCODE_B])
            held = true;
    }
    else if(arg == ST::key::C){
        if(controls.keyboard[SDL_SCANCODE_C] && controls.keyboardFramePrev[SDL_SCANCODE_C])
            held = true;
    }
    else if(arg == ST::key::D){
        if(controls.keyboard[SDL_SCANCODE_D] && controls.keyboardFramePrev[SDL_SCANCODE_D])
            held = true;
    }
    else if(arg == ST::key::E){
        if(controls.keyboard[SDL_SCANCODE_E] && controls.keyboardFramePrev[SDL_SCANCODE_E])
            held = true;
    }
    else if(arg == ST::key::F){
        if(controls.keyboard[SDL_SCANCODE_F] && controls.keyboardFramePrev[SDL_SCANCODE_F])
            held = true;
    }
    else if(arg == ST::key::G){
        if(controls.keyboard[SDL_SCANCODE_G] && controls.keyboardFramePrev[SDL_SCANCODE_G])
            held = true;
    }
    else if(arg == ST::key::H){
        if(controls.keyboard[SDL_SCANCODE_H] && controls.keyboardFramePrev[SDL_SCANCODE_H])
            held = true;
    }
    else if(arg == ST::key::I){
        if(controls.keyboard[SDL_SCANCODE_I] && controls.keyboardFramePrev[SDL_SCANCODE_I])
            held = true;
    }
    else if(arg == ST::key::J){
        if(controls.keyboard[SDL_SCANCODE_J] && controls.keyboardFramePrev[SDL_SCANCODE_G])
            held = true;
    }
    else if(arg == ST::key::K){
        if(controls.keyboard[SDL_SCANCODE_K] && controls.keyboardFramePrev[SDL_SCANCODE_K])
            held = true;
    }
    else if(arg == ST::key::L){
        if(controls.keyboard[SDL_SCANCODE_L] && controls.keyboardFramePrev[SDL_SCANCODE_L])
            held = true;
    }
    else if(arg == ST::key::M){
        if(controls.keyboard[SDL_SCANCODE_M] && controls.keyboardFramePrev[SDL_SCANCODE_M])
            held = true;
    }
    else if(arg == ST::key::N){
        if(controls.keyboard[SDL_SCANCODE_N] && controls.keyboardFramePrev[SDL_SCANCODE_N])
            held = true;
    }
    else if(arg == ST::key::O){
        if(controls.keyboard[SDL_SCANCODE_O] && controls.keyboardFramePrev[SDL_SCANCODE_O])
            held = true;
    }
    else if(arg == ST::key::P){
        if(controls.keyboard[SDL_SCANCODE_P] && controls.keyboardFramePrev[SDL_SCANCODE_P])
            held = true;
    }
    else if(arg == ST::key::Q){
        if(controls.keyboard[SDL_SCANCODE_Q] && controls.keyboardFramePrev[SDL_SCANCODE_Q])
            held = true;
    }
    else if(arg == ST::key::R){
        if(controls.keyboard[SDL_SCANCODE_R] && controls.keyboardFramePrev[SDL_SCANCODE_R])
            held = true;
    }
    else if(arg == ST::key::S){
        if(controls.keyboard[SDL_SCANCODE_S] && controls.keyboardFramePrev[SDL_SCANCODE_S])
            held = true;
    }
    else if(arg == ST::key::T){
        if(controls.keyboard[SDL_SCANCODE_T] && controls.keyboardFramePrev[SDL_SCANCODE_T])
            held = true;
    }
    else if(arg == ST::key::U){
        if(controls.keyboard[SDL_SCANCODE_U] && controls.keyboardFramePrev[SDL_SCANCODE_U])
            held = true;
    }
    else if(arg == ST::key::V){
        if(controls.keyboard[SDL_SCANCODE_V] && controls.keyboardFramePrev[SDL_SCANCODE_V])
            held = true;
    }
    else if(arg == ST::key::W){
        if(controls.keyboard[SDL_SCANCODE_W] && controls.keyboardFramePrev[SDL_SCANCODE_W])
            held = true;
    }
    else if(arg == ST::key::X){
        if(controls.keyboard[SDL_SCANCODE_X] && controls.keyboardFramePrev[SDL_SCANCODE_X])
            held = true;
    }
    else if(arg == ST::key::Y){
        if(controls.keyboard[SDL_SCANCODE_Y] && controls.keyboardFramePrev[SDL_SCANCODE_Y])
            held = true;
    }
    else if(arg == ST::key::Z){
        if(controls.keyboard[SDL_SCANCODE_Z] && controls.keyboardFramePrev[SDL_SCANCODE_Z])
            held = true;
    }
    else if(arg == ST::key::ONE){
        if(controls.keyboard[SDL_SCANCODE_1] && controls.keyboardFramePrev[SDL_SCANCODE_1])
            held = true;
    }
    else if(arg == ST::key::TWO){
        if(controls.keyboard[SDL_SCANCODE_2] && controls.keyboardFramePrev[SDL_SCANCODE_2])
            held = true;
    }
    else if(arg == ST::key::THREE){
        if(controls.keyboard[SDL_SCANCODE_3] && controls.keyboardFramePrev[SDL_SCANCODE_3])
            held = true;
    }
    else if(arg == ST::key::FOUR){
        if(controls.keyboard[SDL_SCANCODE_4] && controls.keyboardFramePrev[SDL_SCANCODE_4])
            held = true;
    }
    else if(arg == ST::key::FIVE){
        if(controls.keyboard[SDL_SCANCODE_5] && controls.keyboardFramePrev[SDL_SCANCODE_5])
            held = true;
    }
    else if(arg == ST::key::SIX){
        if(controls.keyboard[SDL_SCANCODE_6] && controls.keyboardFramePrev[SDL_SCANCODE_6])
            held = true;
    }
    else if(arg == ST::key::SEVEN){
        if(controls.keyboard[SDL_SCANCODE_7] && controls.keyboardFramePrev[SDL_SCANCODE_7])
            held = true;
    }
    else if(arg == ST::key::EIGHT){
        if(controls.keyboard[SDL_SCANCODE_8] && controls.keyboardFramePrev[SDL_SCANCODE_8])
            held = true;
    }
    else if(arg == ST::key::NINE){
        if(controls.keyboard[SDL_SCANCODE_9] && controls.keyboardFramePrev[SDL_SCANCODE_9])
            held = true;
    }
    else if(arg == ST::key::ZERO){
        if(controls.keyboard[SDL_SCANCODE_0] && controls.keyboardFramePrev[SDL_SCANCODE_0])
            held = true;
    }
    else if(arg == ST::key::ESCAPE){
        if(controls.keyboard[SDL_SCANCODE_ESCAPE] && controls.keyboardFramePrev[SDL_SCANCODE_ESCAPE])
            held = true;
    }
    else if(arg == ST::key::ENTER){
        if(controls.keyboard[SDL_SCANCODE_RETURN] && controls.keyboardFramePrev[SDL_SCANCODE_RETURN])
            held = true;
    }
    else if(arg == ST::key::SPACEBAR){
        if(controls.keyboard[SDL_SCANCODE_SPACE] && controls.keyboardFramePrev[SDL_SCANCODE_SPACE])
            held = true;
    }
    else if(arg == ST::key::TILDE){
        if(controls.keyboard[SDL_SCANCODE_GRAVE] && controls.keyboardFramePrev[SDL_SCANCODE_GRAVE])
            held = true;
    }
    else if(arg == ST::key::LSHIFT){
        if(controls.keyboard[SDL_SCANCODE_LSHIFT] && controls.keyboardFramePrev[SDL_SCANCODE_LSHIFT])
            held = true;
    }
    else if(arg == ST::key::BACKSPACE){
        if(controls.keyboard[SDL_SCANCODE_BACKSPACE] && controls.keyboardFramePrev[SDL_SCANCODE_BACKSPACE])
            held = true;
    }
    else if(arg == ST::key::BACKSLASH){
        if(controls.keyboard[SDL_SCANCODE_BACKSLASH] && controls.keyboardFramePrev[SDL_SCANCODE_BACKSLASH])
            held = true;
    }
    else if(arg == ST::key::CAPSLOCK){
        if(controls.keyboard[SDL_SCANCODE_CAPSLOCK] && controls.keyboardFramePrev[SDL_SCANCODE_CAPSLOCK])
            held = true;
    }
    else if(arg == ST::key::COMMA){
        if(controls.keyboard[SDL_SCANCODE_COMMA] && controls.keyboardFramePrev[SDL_SCANCODE_COMMA])
            held = true;
    }
    else if(arg == ST::key::EQUALS){
        if(controls.keyboard[SDL_SCANCODE_EQUALS] && controls.keyboardFramePrev[SDL_SCANCODE_EQUALS])
            held = true;
    }
    else if(arg == ST::key::LALT){
        if(controls.keyboard[SDL_SCANCODE_LALT] && controls.keyboardFramePrev[SDL_SCANCODE_LALT])
            held = true;
    }
    else if(arg == ST::key::LCTRL){
        if(controls.keyboard[SDL_SCANCODE_LCTRL] && controls.keyboardFramePrev[SDL_SCANCODE_LCTRL])
            held = true;
    }
    else if(arg == ST::key::LBRACKET){
        if(controls.keyboard[SDL_SCANCODE_LEFTBRACKET] && controls.keyboardFramePrev[SDL_SCANCODE_LEFTBRACKET])
            held = true;
    }
    else if(arg == ST::key::RBRACKET){
        if(controls.keyboard[SDL_SCANCODE_RIGHTBRACKET] && controls.keyboardFramePrev[SDL_SCANCODE_RIGHTBRACKET])
            held = true;
    }
    else if(arg == ST::key::MINUS){
        if(controls.keyboard[SDL_SCANCODE_MINUS] && controls.keyboardFramePrev[SDL_SCANCODE_MINUS])
            held = true;
    }
    else if(arg == ST::key::RALT){
        if(controls.keyboard[SDL_SCANCODE_RALT] && controls.keyboardFramePrev[SDL_SCANCODE_RALT])
            held = true;
    }
    else if(arg == ST::key::RCTRL){
        if(controls.keyboard[SDL_SCANCODE_RCTRL] && controls.keyboardFramePrev[SDL_SCANCODE_RCTRL])
            held = true;
    }
    else if(arg == ST::key::SEMICOLON){
        if(controls.keyboard[SDL_SCANCODE_SEMICOLON] && controls.keyboardFramePrev[SDL_SCANCODE_SEMICOLON])
            held = true;
    }
    else if(arg == ST::key::SLASH){
        if(controls.keyboard[SDL_SCANCODE_SLASH] && controls.keyboardFramePrev[SDL_SCANCODE_SLASH])
            held = true;
    }
    else if(arg == ST::key::TAB){
        if(controls.keyboard[SDL_SCANCODE_TAB] && controls.keyboardFramePrev[SDL_SCANCODE_TAB])
            held = true;
    }
    else if(arg == ST::key::MOUSELEFT){
        if(controls.mouseClicksFramePrev[0] && controls.mouseClicks[0])
            held = true;
    }
    else if(arg == ST::key::MOUSEMIDDLE){
        if(controls.mouseClicksFramePrev[1] && controls.mouseClicks[1])
            held = true;
    }
    else if(arg == ST::key::MOUSERIGHT){
        if(controls.mouseClicksFramePrev[2] && controls.mouseClicks[2])
            held = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_A){
        if(controller_button_prev_frame.a && controller_buttons.a)
            held = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_B){
        if(controller_button_prev_frame.b && controller_buttons.b)
            held = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_X){
        if(controller_button_prev_frame.x && controller_buttons.x)
            held = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_Y){
        if(controller_button_prev_frame.y && controller_buttons.y)
            held = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_LEFTSTICK){
        if(controller_button_prev_frame.left_stick && controller_buttons.left_stick)
            held = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_RIGHTSTICK){
        if(controller_button_prev_frame.right_stick && controller_buttons.right_stick)
            held = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_DPAD_UP){
        if(controller_button_prev_frame.dpad_up && controller_buttons.dpad_up)
            held = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_DPAD_DOWN){
        if(controller_button_prev_frame.dpad_down && controller_buttons.dpad_down)
            held = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_DPAD_LEFT){
        if(controller_button_prev_frame.dpad_left && controller_buttons.dpad_left)
            held = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_DPAD_RIGHT){
        if(controller_button_prev_frame.dpad_right && controller_buttons.dpad_right)
            held = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_LEFTSHOULDER){
        if(controller_button_prev_frame.left_shoulder && controller_buttons.left_shoulder)
            held = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_RIGHTSHOULDER){
        if(controller_button_prev_frame.right_shoulder && controller_buttons.right_shoulder)
            held = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_SELECT){
        if(controller_button_prev_frame.select && controller_buttons.select)
            held = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_START){
        if(controller_button_prev_frame.start && controller_buttons.start)
            held = true;
    }
    return held;
}

/**
 * Tells if a key is released or not. Works by comparing it's
 * current state with the state from the previous frame.
 * @param arg an <b>ST::key</b> to key for being released.
 * @return True if released, false otherwise.
 */
bool input_manager::keyrelease(ST::key arg) const{
    bool released = false;
    if(arg == ST::key::LEFT){
        if(!controls.keyboard[SDL_SCANCODE_LEFT] && controls.keyboardFramePrev[SDL_SCANCODE_LEFT])
            released = true;
    }
    else if(arg == ST::key::RIGHT){
        if(!controls.keyboard[SDL_SCANCODE_RIGHT] && controls.keyboardFramePrev[SDL_SCANCODE_RIGHT])
            released = true;
    }
    else if(arg == ST::key::UP){
        if(!controls.keyboard[SDL_SCANCODE_UP] && controls.keyboardFramePrev[SDL_SCANCODE_UP])
            released = true;
    }
    else if(arg == ST::key::DOWN){
        if(!controls.keyboard[SDL_SCANCODE_DOWN] && controls.keyboardFramePrev[SDL_SCANCODE_DOWN])
            released = true;
    }
    else if(arg == ST::key::A){
        if(!controls.keyboard[SDL_SCANCODE_A] && controls.keyboardFramePrev[SDL_SCANCODE_A])
            released = true;
    }
    else if(arg == ST::key::B){
        if(!controls.keyboard[SDL_SCANCODE_B] && controls.keyboardFramePrev[SDL_SCANCODE_B])
            released = true;
    }
    else if(arg == ST::key::C){
        if(!controls.keyboard[SDL_SCANCODE_C] && controls.keyboardFramePrev[SDL_SCANCODE_C])
            released = true;
    }
    else if(arg == ST::key::D){
        if(!controls.keyboard[SDL_SCANCODE_D] && controls.keyboardFramePrev[SDL_SCANCODE_D])
            released = true;
    }
    else if(arg == ST::key::E){
        if(!controls.keyboard[SDL_SCANCODE_E] && controls.keyboardFramePrev[SDL_SCANCODE_E])
            released = true;
    }
    else if(arg == ST::key::F){
        if(!controls.keyboard[SDL_SCANCODE_F] && controls.keyboardFramePrev[SDL_SCANCODE_F])
            released = true;
    }
    else if(arg == ST::key::G){
        if(!controls.keyboard[SDL_SCANCODE_G] && controls.keyboardFramePrev[SDL_SCANCODE_G])
            released = true;
    }
    else if(arg == ST::key::H){
        if(!controls.keyboard[SDL_SCANCODE_H] && controls.keyboardFramePrev[SDL_SCANCODE_H])
            released = true;
    }
    else if(arg == ST::key::I){
        if(!controls.keyboard[SDL_SCANCODE_I] && controls.keyboardFramePrev[SDL_SCANCODE_I])
            released = true;
    }
    else if(arg == ST::key::J){
        if(!controls.keyboard[SDL_SCANCODE_J] && controls.keyboardFramePrev[SDL_SCANCODE_G])
            released = true;
    }
    else if(arg == ST::key::K){
        if(!controls.keyboard[SDL_SCANCODE_K] && controls.keyboardFramePrev[SDL_SCANCODE_K])
            released = true;
    }
    else if(arg == ST::key::L){
        if(!controls.keyboard[SDL_SCANCODE_L] && controls.keyboardFramePrev[SDL_SCANCODE_L])
            released = true;
    }
    else if(arg == ST::key::M){
        if(!controls.keyboard[SDL_SCANCODE_M] && controls.keyboardFramePrev[SDL_SCANCODE_M])
            released = true;
    }
    else if(arg == ST::key::N){
        if(!controls.keyboard[SDL_SCANCODE_N] && controls.keyboardFramePrev[SDL_SCANCODE_N])
            released = true;
    }
    else if(arg == ST::key::O){
        if(!controls.keyboard[SDL_SCANCODE_O] && controls.keyboardFramePrev[SDL_SCANCODE_O])
            released = true;
    }
    else if(arg == ST::key::P){
        if(!controls.keyboard[SDL_SCANCODE_P] && controls.keyboardFramePrev[SDL_SCANCODE_P])
            released = true;
    }
    else if(arg == ST::key::Q){
        if(!controls.keyboard[SDL_SCANCODE_Q] && controls.keyboardFramePrev[SDL_SCANCODE_Q])
            released = true;
    }
    else if(arg == ST::key::R){
        if(!controls.keyboard[SDL_SCANCODE_R] && controls.keyboardFramePrev[SDL_SCANCODE_R])
            released = true;
    }
    else if(arg == ST::key::S){
        if(!controls.keyboard[SDL_SCANCODE_S] && controls.keyboardFramePrev[SDL_SCANCODE_S])
            released = true;
    }
    else if(arg == ST::key::T){
        if(!controls.keyboard[SDL_SCANCODE_T] && controls.keyboardFramePrev[SDL_SCANCODE_T])
            released = true;
    }
    else if(arg == ST::key::U){
        if(!controls.keyboard[SDL_SCANCODE_U] && controls.keyboardFramePrev[SDL_SCANCODE_U])
            released = true;
    }
    else if(arg == ST::key::V){
        if(!controls.keyboard[SDL_SCANCODE_V] && controls.keyboardFramePrev[SDL_SCANCODE_V])
            released = true;
    }
    else if(arg == ST::key::W){
        if(!controls.keyboard[SDL_SCANCODE_W] && controls.keyboardFramePrev[SDL_SCANCODE_W])
            released = true;
    }
    else if(arg == ST::key::X){
        if(!controls.keyboard[SDL_SCANCODE_X] && controls.keyboardFramePrev[SDL_SCANCODE_X])
            released = true;
    }
    else if(arg == ST::key::Y){
        if(!controls.keyboard[SDL_SCANCODE_Y] && controls.keyboardFramePrev[SDL_SCANCODE_Y])
            released = true;
    }
    else if(arg == ST::key::Z){
        if(!controls.keyboard[SDL_SCANCODE_Z] && controls.keyboardFramePrev[SDL_SCANCODE_Z])
            released = true;
    }
    else if(arg == ST::key::ONE){
        if(!controls.keyboard[SDL_SCANCODE_1] && controls.keyboardFramePrev[SDL_SCANCODE_1])
            released = true;
    }
    else if(arg == ST::key::TWO){
        if(!controls.keyboard[SDL_SCANCODE_2] && controls.keyboardFramePrev[SDL_SCANCODE_2])
            released = true;
    }
    else if(arg == ST::key::THREE){
        if(!controls.keyboard[SDL_SCANCODE_3] && controls.keyboardFramePrev[SDL_SCANCODE_3])
            released = true;
    }
    else if(arg == ST::key::FOUR){
        if(!controls.keyboard[SDL_SCANCODE_4] && controls.keyboardFramePrev[SDL_SCANCODE_4])
            released = true;
    }
    else if(arg == ST::key::FIVE){
        if(!controls.keyboard[SDL_SCANCODE_5] && controls.keyboardFramePrev[SDL_SCANCODE_5])
            released = true;
    }
    else if(arg == ST::key::SIX){
        if(!controls.keyboard[SDL_SCANCODE_6] && controls.keyboardFramePrev[SDL_SCANCODE_6])
            released = true;
    }
    else if(arg == ST::key::SEVEN){
        if(!controls.keyboard[SDL_SCANCODE_7] && controls.keyboardFramePrev[SDL_SCANCODE_7])
            released = true;
    }
    else if(arg == ST::key::EIGHT){
        if(!controls.keyboard[SDL_SCANCODE_8] && controls.keyboardFramePrev[SDL_SCANCODE_8])
            released = true;
    }
    else if(arg == ST::key::NINE){
        if(!controls.keyboard[SDL_SCANCODE_9] && controls.keyboardFramePrev[SDL_SCANCODE_9])
            released = true;
    }
    else if(arg == ST::key::ZERO){
        if(!controls.keyboard[SDL_SCANCODE_0] && controls.keyboardFramePrev[SDL_SCANCODE_0])
            released = true;
    }
    else if(arg == ST::key::ESCAPE){
        if(!controls.keyboard[SDL_SCANCODE_ESCAPE] && controls.keyboardFramePrev[SDL_SCANCODE_ESCAPE])
            released = true;
    }
    else if(arg == ST::key::ENTER){
        if(!controls.keyboard[SDL_SCANCODE_RETURN] && controls.keyboardFramePrev[SDL_SCANCODE_RETURN])
            released = true;
    }
    else if(arg == ST::key::SPACEBAR){
        if(!controls.keyboard[SDL_SCANCODE_SPACE] && controls.keyboardFramePrev[SDL_SCANCODE_SPACE])
            released = true;
    }
    else if(arg == ST::key::TILDE){
        if(!controls.keyboard[SDL_SCANCODE_GRAVE] && controls.keyboardFramePrev[SDL_SCANCODE_GRAVE])
            released = true;
    }
    else if(arg == ST::key::LSHIFT){
        if(!controls.keyboard[SDL_SCANCODE_LSHIFT] && controls.keyboardFramePrev[SDL_SCANCODE_LSHIFT])
            released = true;
    }
    else if(arg == ST::key::BACKSPACE){
        if(!controls.keyboard[SDL_SCANCODE_BACKSPACE] && controls.keyboardFramePrev[SDL_SCANCODE_BACKSPACE])
            released = true;
    }
    else if(arg == ST::key::BACKSLASH){
        if(!controls.keyboard[SDL_SCANCODE_BACKSLASH] && controls.keyboardFramePrev[SDL_SCANCODE_BACKSLASH])
            released = true;
    }
    else if(arg == ST::key::CAPSLOCK){
        if(!controls.keyboard[SDL_SCANCODE_CAPSLOCK] && controls.keyboardFramePrev[SDL_SCANCODE_CAPSLOCK])
            released = true;
    }
    else if(arg == ST::key::COMMA){
        if(!controls.keyboard[SDL_SCANCODE_COMMA] && controls.keyboardFramePrev[SDL_SCANCODE_COMMA])
            released = true;
    }
    else if(arg == ST::key::EQUALS){
        if(!controls.keyboard[SDL_SCANCODE_EQUALS] && controls.keyboardFramePrev[SDL_SCANCODE_EQUALS])
            released = true;
    }
    else if(arg == ST::key::LALT){
        if(!controls.keyboard[SDL_SCANCODE_LALT] && controls.keyboardFramePrev[SDL_SCANCODE_LALT])
            released = true;
    }
    else if(arg == ST::key::LCTRL){
        if(!controls.keyboard[SDL_SCANCODE_LCTRL] && controls.keyboardFramePrev[SDL_SCANCODE_LCTRL])
            released = true;
    }
    else if(arg == ST::key::LBRACKET){
        if(!controls.keyboard[SDL_SCANCODE_LEFTBRACKET] && controls.keyboardFramePrev[SDL_SCANCODE_LEFTBRACKET])
            released = true;
    }
    else if(arg == ST::key::RBRACKET){
        if(!controls.keyboard[SDL_SCANCODE_RIGHTBRACKET] && controls.keyboardFramePrev[SDL_SCANCODE_RIGHTBRACKET])
            released = true;
    }
    else if(arg == ST::key::MINUS){
        if(!controls.keyboard[SDL_SCANCODE_MINUS] && controls.keyboardFramePrev[SDL_SCANCODE_MINUS])
            released = true;
    }
    else if(arg == ST::key::RALT){
        if(!controls.keyboard[SDL_SCANCODE_RALT] && controls.keyboardFramePrev[SDL_SCANCODE_RALT])
            released = true;
    }
    else if(arg == ST::key::RCTRL){
        if(!controls.keyboard[SDL_SCANCODE_RCTRL] && controls.keyboardFramePrev[SDL_SCANCODE_RCTRL])
            released = true;
    }
    else if(arg == ST::key::SEMICOLON){
        if(!controls.keyboard[SDL_SCANCODE_SEMICOLON] && controls.keyboardFramePrev[SDL_SCANCODE_SEMICOLON])
            released = true;
    }
    else if(arg == ST::key::SLASH){
        if(!controls.keyboard[SDL_SCANCODE_SLASH] && controls.keyboardFramePrev[SDL_SCANCODE_SLASH])
            released = true;
    }
    else if(arg == ST::key::TAB){
        if(!controls.keyboard[SDL_SCANCODE_TAB] && controls.keyboardFramePrev[SDL_SCANCODE_TAB])
            released = true;
    }
    else if(arg == ST::key::MOUSELEFT){
        if(controls.mouseClicksFramePrev[0] && !controls.mouseClicks[0])
            released = true;
    }
    else if(arg == ST::key::MOUSEMIDDLE){
        if(controls.mouseClicksFramePrev[1] && !controls.mouseClicks[1])
            released = true;
    }
    else if(arg == ST::key::MOUSERIGHT){
        if(controls.mouseClicksFramePrev[2] && !controls.mouseClicks[2])
            released = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_A){
        if(controller_button_prev_frame.a && !controller_buttons.a)
            released = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_B){
        if(controller_button_prev_frame.b && !controller_buttons.b)
            released = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_X){
        if(controller_button_prev_frame.x && !controller_buttons.x)
            released = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_Y){
        if(controller_button_prev_frame.y && !controller_buttons.y)
            released = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_LEFTSTICK){
        if(controller_button_prev_frame.left_stick && !controller_buttons.left_stick)
            released = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_RIGHTSTICK){
        if(controller_button_prev_frame.right_stick && !controller_buttons.right_stick)
            released = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_DPAD_UP){
        if(controller_button_prev_frame.dpad_up && !controller_buttons.dpad_up)
            released = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_DPAD_DOWN){
        if(controller_button_prev_frame.dpad_down && !controller_buttons.dpad_down)
            released = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_DPAD_LEFT){
        if(controller_button_prev_frame.dpad_left && !controller_buttons.dpad_left)
            released = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_DPAD_RIGHT){
        if(controller_button_prev_frame.dpad_right && !controller_buttons.dpad_right)
            released = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_LEFTSHOULDER){
        if(controller_button_prev_frame.left_shoulder && !controller_buttons.left_shoulder)
            released = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_RIGHTSHOULDER){
        if(controller_button_prev_frame.right_shoulder && !controller_buttons.right_shoulder)
            released = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_SELECT){
        if(controller_button_prev_frame.select && !controller_buttons.select)
            released = true;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_START){
        if(controller_button_prev_frame.start && !controller_buttons.start)
            released = true;
    }
    return released;
}