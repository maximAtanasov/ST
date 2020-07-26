/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#include <input_manager.hpp>
#include <SDL.h>
#include <string>



static bool singleton_initialized = false;

input_manager::~input_manager() {
    singleton_initialized = false;
}

/**
 * initializes the input manager
 * @param msg_bus A pointer to the global message bus.
 * @param tsk_mngr A pointer to the global task_manager.
 */
input_manager::input_manager(task_manager &gTask_manager, message_bus &gMessageBus) : gMessage_bus(gMessageBus), gTask_manager(gTask_manager) {

    if(singleton_initialized){
        throw std::runtime_error("The input manager cannot be initialized more than once!");
    }else{
        singleton_initialized = true;
    }

    if( SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC) < 0 ){
        gMessage_bus.send_msg(new message(LOG_ERROR, make_data<std::string>("Could not initialize gamepad subsystem!")));
    }

    //Initialize controls
    int32_t length = 0;
    controls.keyboard = reinterpret_cast<bool*>(const_cast<uint8_t*>(SDL_GetKeyboardState(&length)));
    controls_prev_frame.keyboard = static_cast<bool*>(malloc(static_cast<size_t>(length)));
    SDL_PollEvent(&event);

    //Subscribe to the messages we need
	gMessage_bus.subscribe(VIRTUAL_SCREEN_COORDINATES, &msg_sub);
	gMessage_bus.subscribe(REAL_SCREEN_COORDINATES, &msg_sub);
    gMessage_bus.subscribe(START_TEXT_INPUT, &msg_sub);
    gMessage_bus.subscribe(STOP_TEXT_INPUT, &msg_sub);
    gMessage_bus.subscribe(CLEAR_TEXT_STREAM, &msg_sub);
    gMessage_bus.subscribe(REGISTER_KEY, &msg_sub);
    gMessage_bus.subscribe(UNREGISTER_KEY, &msg_sub);
    gMessage_bus.subscribe(CONTROLLER_RUMBLE, &msg_sub);
    gMessage_bus.subscribe(SET_LEFT_JOYSTICK_HORIZONTAL_THRESHOLD, &msg_sub);
    gMessage_bus.subscribe(SET_LEFT_JOYSTICK_VERTICAL_THRESHOLD, &msg_sub);
    gMessage_bus.subscribe(SET_RIGHT_JOYSTICK_HORIZONTAL_THRESHOLD, &msg_sub);
    gMessage_bus.subscribe(SET_RIGHT_JOYSTICK_VERTICAL_THRESHOLD, &msg_sub);
    gMessage_bus.subscribe(SET_LEFT_TRIGGER_THRESHOLD, &msg_sub);
    gMessage_bus.subscribe(SET_RIGHT_TRIGGER_THRESHOLD, &msg_sub);
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
 * Checks the state of the keyboard and any incoming events and sends appropriate messages.
 */
void input_manager::take_input(){

    memcpy(controls_prev_frame.keyboard, controls.keyboard, 512);
    take_controller_input();
    take_mouse_input();

    while(SDL_PollEvent(&event) != 0){
        if(event.type == SDL_QUIT){
            gMessage_bus.send_msg(new message(END_GAME));
        }
        if(event.type == SDL_MOUSEWHEEL){
            controls.mouse_scroll += event.wheel.y;
            if(controls.mouse_scroll < 0){
                controls.mouse_scroll = 0;
            }
            gMessage_bus.send_msg(new message(MOUSE_SCROLL, controls.mouse_scroll));
        }
        if(event.type == SDL_TEXTINPUT){
            if(text_input) {
                composition += event.edit.text;
                //Drop the tilde when closing the console, otherwise it just stays there
                while(!composition.empty() && composition.at(composition.size()-1) == CONSOLE_TOGGLE_KEY){
                    composition.pop_back();
                }
                gMessage_bus.send_msg(new message(TEXT_STREAM, make_data(composition)));
            }
        }
        if(event.type == SDL_KEYDOWN){
            if(event.key.keysym.sym == SDLK_BACKSPACE) {
                if (composition.length() > 0) {
                    composition.pop_back();
                    gMessage_bus.send_msg(new message(TEXT_STREAM, make_data(composition)));
                }
            }
        }
		if(event.type == SDL_MOUSEMOTION){
			SDL_GetMouseState(&controls.mouse_x, &controls.mouse_y);
			controls.mouse_x = static_cast<int>(static_cast<float>(controls.mouse_x)*ratio_w);
			controls.mouse_y = static_cast<int>(static_cast<float>(controls.mouse_y)*ratio_h);
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
			gMessage_bus.send_msg(new message(LOG_INFO, make_data<std::string>("Found a controller: " + std::string(SDL_GameControllerName(controller)))));
			SDL_Haptic* haptic = SDL_HapticOpen(static_cast<int32_t>(controllers.size() - 1));
			if (haptic != nullptr) {
				if (SDL_HapticRumbleInit(haptic) < 0){
					gMessage_bus.send_msg(new message(LOG_INFO, make_data<std::string>(
						"Unable to initialize rumble for controller " + std::string(SDL_GameControllerName(controller)))));
				}
				else {
                    gMessage_bus.send_msg(new message(LOG_INFO, make_data<std::string>(
                            "The controller \"" + std::string(SDL_GameControllerName(controller)) +
                            "\" supports haptic feedback")));
                    controllers_haptic.emplace_back(haptic);
				}
			}
			else {
				gMessage_bus.send_msg(new message(LOG_INFO, make_data<std::string>(
					"The controller \"" + std::string(SDL_GameControllerName(controller)) + 
					"\" does not support haptic feedback")));		
			}
        }
        if(event.cdevice.type == SDL_CONTROLLERDEVICEREMOVED){
            uint8_t number = 0;
            for(uint8_t i = 0; i < controllers.size() && i < 8; i++){
                if(SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controllers.at(i))) == event.cdevice.which){
                    number = i;
                }
            }
            SDL_GameControllerClose(controllers.at(number));
            controllers.erase(controllers.begin() + number);
			if (number < controllers_haptic.size()) {
				SDL_HapticClose(controllers_haptic.at(number));
				controllers_haptic.erase(controllers_haptic.begin() + number);
			}
            gMessage_bus.send_msg(new message(LOG_INFO, make_data<std::string>("Controller " + std::to_string(number+1) + " disconnected")));
        }
    }

    //check if any of the registered keys is pressed and send a message if so
    for(auto i : registered_keys){
        if(i.second > 0) {
            if (key_event(i.first, ST::key_event::PRESS)) {
                gMessage_bus.send_msg(new message(KEY_PRESSED, static_cast<uint8_t>(i.first)));
            }
            if (key_event(i.first, ST::key_event::HOLD)) {
                gMessage_bus.send_msg(new message(KEY_HELD, static_cast<uint8_t>(i.first)));
            }
            if (key_event(i.first, ST::key_event::RELEASE)) {
                gMessage_bus.send_msg(new message(KEY_RELEASED, static_cast<uint8_t>(i.first)));
            }
        }
    }
}

#pragma clang diagnostic push
void input_manager::take_mouse_input() {
    //Collect mouse input
    controls_prev_frame.mouse_clicks[0] = controls.mouse_clicks[0];
    controls_prev_frame.mouse_clicks[1] = controls.mouse_clicks[1];
    controls_prev_frame.mouse_clicks[2] = controls.mouse_clicks[2];
    controls.mouse_clicks[0] = SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT);
    controls.mouse_clicks[1] = SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_MIDDLE);
    controls.mouse_clicks[2] = SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_RIGHT);

    //only send mouse coordinates if they change
    if(controls.mouse_x != controls_prev_frame.mouse_x){
        gMessage_bus.send_msg(new message(MOUSE_X, static_cast<uint32_t>(controls.mouse_x)));
        controls_prev_frame.mouse_x = controls.mouse_x;
    }
    if(controls.mouse_y != controls_prev_frame.mouse_y){
        gMessage_bus.send_msg(new message(MOUSE_Y, static_cast<uint32_t>(controls.mouse_y)));
        controls_prev_frame.mouse_y = controls.mouse_y;
    }
}
#pragma clang diagnostic pop

/**
 * Takes input from all available controllers and
 * send messages if any analog inputs are clicked.
 * Messages for controller buttons are sent in the take input method.
 */
void input_manager::take_controller_input(){
    controller_button_prev_frame = controller_buttons;
    for(SDL_GameController* c : controllers) {
        controller_buttons.a = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_A);
        controller_buttons.b = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_B);
        controller_buttons.x = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_X);
        controller_buttons.y = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_Y);
        controller_buttons.dpad_up = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_DPAD_UP);
        controller_buttons.dpad_down = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
        controller_buttons.dpad_left = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
        controller_buttons.dpad_right = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
        controller_buttons.left_stick = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_LEFTSTICK);
        controller_buttons.right_stick = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_RIGHTSTICK);
        controller_buttons.left_shoulder = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
        controller_buttons.right_shoulder = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
        controller_buttons.start = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_START);
        controller_buttons.select = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_BACK);

        controller_buttons.left_trigger = SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
        controller_buttons.right_trigger = SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
        controller_buttons.right_stick_horizontal = SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_RIGHTX);
        controller_buttons.right_stick_vertical = SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_RIGHTY);
        controller_buttons.left_stick_horizontal = SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_LEFTX);
        controller_buttons.left_stick_vertical = SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_LEFTY);
    }
    //only send controller axis values if they change and the values exceed the thresholds

    if(controller_buttons.left_trigger < left_trigger_threshold){
        controller_buttons.left_trigger = 0;
    }
    if(controller_buttons.left_trigger != controller_button_prev_frame.left_trigger){
        gMessage_bus.send_msg(new message(LEFT_TRIGGER, controller_buttons.left_trigger));
    }

    if(controller_buttons.right_trigger < right_trigger_threshold){
        controller_buttons.right_trigger = 0;
    }
    if(controller_buttons.right_trigger != controller_button_prev_frame.right_trigger){
        gMessage_bus.send_msg(new message(RIGHT_TRIGGER, controller_buttons.right_trigger));
    }

    if(!(controller_buttons.left_stick_vertical > left_stick_vertical_threshold
         || controller_buttons.left_stick_vertical < -left_stick_vertical_threshold)){
        controller_buttons.right_stick_vertical = 0;
    }
    if(controller_buttons.left_stick_vertical != controller_button_prev_frame.left_stick_vertical){
        gMessage_bus.send_msg(new message(LEFT_STICK_VERTICAL, controller_buttons.left_stick_vertical));
    }

    if(!(controller_buttons.left_stick_horizontal > left_stick_horizontal_threshold
        || controller_buttons.left_stick_horizontal < -left_stick_horizontal_threshold)){
        controller_buttons.left_stick_horizontal = 0;
    }
    if(controller_buttons.left_stick_horizontal != controller_button_prev_frame.left_stick_horizontal){
        gMessage_bus.send_msg(new message(LEFT_STICK_HORIZONTAL, controller_buttons.left_stick_horizontal));
    }

    if(!(controller_buttons.right_stick_vertical > right_stick_vertical_threshold
         || controller_buttons.right_stick_vertical < -right_stick_vertical_threshold)){
        controller_buttons.right_stick_vertical = 0;
    }
    if(controller_buttons.right_stick_vertical != controller_button_prev_frame.right_stick_vertical){
        gMessage_bus.send_msg(new message(RIGHT_STICK_VERTICAL, controller_buttons.right_stick_vertical));
    }

    if(!(controller_buttons.right_stick_horizontal > right_stick_horizontal_threshold
         || controller_buttons.right_stick_horizontal < -right_stick_horizontal_threshold)){
        controller_buttons.right_stick_horizontal = 0;
    }
    if(controller_buttons.right_stick_horizontal != controller_button_prev_frame.right_stick_horizontal){
        gMessage_bus.send_msg(new message(RIGHT_STICK_HORIZONTAL, controller_buttons.right_stick_horizontal));
    }
}

/**
 * Retrieves messages from the subscriber object and
 * performs the appropriate actions.
 */
void input_manager::handle_messages(){
	message* temp = msg_sub.get_next_message();
	while(temp != nullptr){
        switch (temp->msg_name) {
            case VIRTUAL_SCREEN_COORDINATES: {
                auto data = temp->base_data0;
                v_width = data & 0x0000ffffU;
                v_height = (data >> 16U) & 0x0000ffffU;
                ratio_w = static_cast<float>(v_width) / static_cast<float>(r_width);
                ratio_h = static_cast<float>(v_height) / static_cast<float>(r_height);
                break;
            }
            case REAL_SCREEN_COORDINATES: {
                auto data = temp->base_data0;
                r_width = data & 0x0000ffffU;
                r_height = (data >> 16U) & 0x0000ffffU;
                ratio_w = static_cast<float>(v_width) / static_cast<float>(r_width);
                ratio_h = static_cast<float>(v_height) / static_cast<float>(r_height);
                break;
            }
            case START_TEXT_INPUT:
                text_input = true;
                break;
            case STOP_TEXT_INPUT:
                text_input = false;
                break;
            case CLEAR_TEXT_STREAM:
                composition.clear();
                break;
            case REGISTER_KEY: {
                auto key_val = static_cast<ST::key>(temp->base_data0);
                ++registered_keys[key_val];
                break;
            }
            case UNREGISTER_KEY: {
                auto key_val = static_cast<ST::key>(temp->base_data0);
                if (registered_keys[key_val] > 0) {
                    --registered_keys[key_val];
                }
                break;
            }
            case CONTROLLER_RUMBLE: {
                if(!controllers.empty() && !controllers_haptic.empty()){
                    float strength = *reinterpret_cast<float*>(&temp->base_data0);
                    uint16_t duration = temp->base_data1;
                    for(SDL_Haptic* haptic : controllers_haptic){
                        SDL_HapticRumblePlay(haptic, strength, duration);
                    }
                }
                break;
            }
            case SET_LEFT_JOYSTICK_HORIZONTAL_THRESHOLD:
                this->left_stick_horizontal_threshold = static_cast<int16_t>(temp->base_data0);
                break;
            case SET_LEFT_JOYSTICK_VERTICAL_THRESHOLD:
                this->left_stick_vertical_threshold = static_cast<int16_t>(temp->base_data0);
                break;
            case SET_RIGHT_JOYSTICK_HORIZONTAL_THRESHOLD:
                this->right_stick_horizontal_threshold = static_cast<int16_t>(temp->base_data0);
                break;
            case SET_RIGHT_JOYSTICK_VERTICAL_THRESHOLD:
                this->right_stick_vertical_threshold = static_cast<int16_t>(temp->base_data0);
                break;
            case SET_RIGHT_TRIGGER_THRESHOLD:
                this->right_trigger_threshold = static_cast<int16_t>(temp->base_data0);
                break;
            case SET_LEFT_TRIGGER_THRESHOLD:
                this->left_trigger_threshold = static_cast<int16_t>(temp->base_data0);
                break;
        }
		delete temp;
		temp = msg_sub.get_next_message();
	}
}

/**
 * Tells if a key's state corresponds to the k_event. Works by comparing it's
 * current state with the state from the previous frame.
 * @param arg an <b>ST::key</b> to test.
 * @param k_event an <b>ST::key_event</b> to test for.
 * @return True if event k_event has happened for key arg, false otherwise.
 */
bool input_manager::key_event(ST::key arg, ST::key_event k_event) const{
    bool pressed = false;
    bool first = k_event == ST::key_event::RELEASE;
    bool second = k_event == ST::key_event::PRESS;

    switch (arg) {
        case ST::key::LEFT:
            pressed = (controls.keyboard[SDL_SCANCODE_LEFT] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_LEFT] ^ second);
            break;
        case ST::key::RIGHT:
            pressed = (controls.keyboard[SDL_SCANCODE_RIGHT] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_RIGHT] ^ second);
            break;
        case ST::key::UP:
            pressed = (controls.keyboard[SDL_SCANCODE_UP] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_UP] ^ second);
            break;
        case ST::key::DOWN:
            pressed = (controls.keyboard[SDL_SCANCODE_DOWN] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_DOWN] ^ second);
            break;
        case ST::key::A:
            pressed = (controls.keyboard[SDL_SCANCODE_A] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_A] ^ second);
            break;
        case ST::key::B:
            pressed = (controls.keyboard[SDL_SCANCODE_B] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_B] ^ second);
            break;
        case ST::key::C:
            pressed = (controls.keyboard[SDL_SCANCODE_C] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_C] ^ second);
            break;
        case ST::key::D:
            pressed = (controls.keyboard[SDL_SCANCODE_D] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_D] ^ second);
            break;
        case ST::key::E:
            pressed = (controls.keyboard[SDL_SCANCODE_E] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_E] ^ second);
            break;
        case ST::key::F:
            pressed = (controls.keyboard[SDL_SCANCODE_F] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_F] ^ second);
            break;
        case ST::key::G:
            pressed = (controls.keyboard[SDL_SCANCODE_G] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_G] ^ second);
            break;
        case ST::key::H:
            pressed = (controls.keyboard[SDL_SCANCODE_H] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_H] ^ second);
            break;
        case ST::key::I:
            pressed = (controls.keyboard[SDL_SCANCODE_I] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_I] ^ second);
            break;
        case ST::key::J:
            pressed = (controls.keyboard[SDL_SCANCODE_J] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_J] ^ second);
            break;
        case ST::key::K:
            pressed = (controls.keyboard[SDL_SCANCODE_K] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_K] ^ second);
            break;
        case ST::key::L:
            pressed = (controls.keyboard[SDL_SCANCODE_L] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_L] ^ second);
            break;
        case ST::key::M:
            pressed = (controls.keyboard[SDL_SCANCODE_M] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_M] ^ second);
            break;
        case ST::key::N:
            pressed = (controls.keyboard[SDL_SCANCODE_N] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_N] ^ second);
            break;
        case ST::key::O:
            pressed = (controls.keyboard[SDL_SCANCODE_O] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_RIGHT] ^ second);
            break;
        case ST::key::P:
            pressed = (controls.keyboard[SDL_SCANCODE_P] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_P] ^ second);
            break;
        case ST::key::Q:
            pressed = (controls.keyboard[SDL_SCANCODE_Q] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_Q] ^ second);
            break;
        case ST::key::R:
            pressed = (controls.keyboard[SDL_SCANCODE_R] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_R] ^ second);
            break;
        case ST::key::S:
            pressed = (controls.keyboard[SDL_SCANCODE_S] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_S] ^ second);
            break;
        case ST::key::T:
            pressed = (controls.keyboard[SDL_SCANCODE_T] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_T] ^ second);
            break;
        case ST::key::V:
            pressed = (controls.keyboard[SDL_SCANCODE_V] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_V] ^ second);
            break;
        case ST::key::U:
            pressed = (controls.keyboard[SDL_SCANCODE_U] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_U] ^ second);
            break;
        case ST::key::W:
            pressed = (controls.keyboard[SDL_SCANCODE_W] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_W] ^ second);
            break;
        case ST::key::X:
            pressed = (controls.keyboard[SDL_SCANCODE_X] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_X] ^ second);
            break;
        case ST::key::Y:
            pressed = (controls.keyboard[SDL_SCANCODE_Y] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_Y] ^ second);
            break;
        case ST::key::Z:
            pressed = (controls.keyboard[SDL_SCANCODE_Z] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_Z] ^ second);
            break;
        case ST::key::ONE:
            pressed = (controls.keyboard[SDL_SCANCODE_1] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_1] ^ second);
            break;
        case ST::key::TWO:
            pressed = (controls.keyboard[SDL_SCANCODE_2] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_2] ^ second);
            break;
        case ST::key::THREE:
            pressed = (controls.keyboard[SDL_SCANCODE_3] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_3] ^ second);
            break;
        case ST::key::FOUR:
            pressed = (controls.keyboard[SDL_SCANCODE_4] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_4] ^ second);
            break;
        case ST::key::FIVE:
            pressed = (controls.keyboard[SDL_SCANCODE_5] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_5] ^ second);
            break;
        case ST::key::SIX:
            pressed = (controls.keyboard[SDL_SCANCODE_6] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_6] ^ second);
            break;
        case ST::key::SEVEN:
            pressed = (controls.keyboard[SDL_SCANCODE_7] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_7] ^ second);
            break;
        case ST::key::EIGHT:
            pressed = (controls.keyboard[SDL_SCANCODE_8] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_8] ^ second);
            break;
        case ST::key::NINE:
            pressed = (controls.keyboard[SDL_SCANCODE_9] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_9] ^ second);
            break;
        case ST::key::ZERO:
            pressed = (controls.keyboard[SDL_SCANCODE_0] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_0] ^ second);
            break;
        case ST::key::ESCAPE:
            pressed = (controls.keyboard[SDL_SCANCODE_ESCAPE] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_ESCAPE] ^ second);
            break;
        case ST::key::ENTER:
            pressed = (controls.keyboard[SDL_SCANCODE_RETURN] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_RETURN] ^ second);
            break;
        case ST::key::SPACEBAR:
            pressed = (controls.keyboard[SDL_SCANCODE_SPACE] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_SPACE] ^ second);
            break;
        case ST::key::TILDE:
            pressed = (controls.keyboard[SDL_SCANCODE_GRAVE] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_GRAVE] ^ second);
            break;
        case ST::key::LSHIFT:
            pressed = (controls.keyboard[SDL_SCANCODE_LSHIFT] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_LSHIFT] ^ second);
            break;
        case ST::key::BACKSPACE:
            pressed = (controls.keyboard[SDL_SCANCODE_BACKSPACE] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_BACKSPACE] ^ second);
            break;
        case ST::key::DELETE:
            pressed = (controls.keyboard[SDL_SCANCODE_DELETE] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_DELETE] ^ second);
            break;
        case ST::key::BACKSLASH:
            pressed = (controls.keyboard[SDL_SCANCODE_BACKSLASH] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_BACKSLASH] ^ second);
            break;
        case ST::key::CAPSLOCK:
            pressed = (controls.keyboard[SDL_SCANCODE_CAPSLOCK] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_CAPSLOCK] ^ second);
            break;
        case ST::key::COMMA:
            pressed = (controls.keyboard[SDL_SCANCODE_COMMA] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_COMMA] ^ second);
            break;
        case ST::key::EQUALS:
            pressed = (controls.keyboard[SDL_SCANCODE_EQUALS] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_EQUALS] ^ second);
            break;
        case ST::key::LALT:
            pressed = (controls.keyboard[SDL_SCANCODE_LALT] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_LALT] ^ second);
            break;
        case ST::key::LCTRL:
            pressed = (controls.keyboard[SDL_SCANCODE_LCTRL] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_LCTRL] ^ second);
            break;
        case ST::key::LBRACKET:
            pressed = (controls.keyboard[SDL_SCANCODE_LEFTBRACKET] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_LEFTBRACKET] ^ second);
            break;
        case ST::key::RBRACKET:
            pressed = (controls.keyboard[SDL_SCANCODE_RIGHTBRACKET] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_RIGHTBRACKET] ^ second);
            break;
        case ST::key::MINUS:
            pressed = (controls.keyboard[SDL_SCANCODE_MINUS] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_MINUS] ^ second);
            break;
        case ST::key::RALT:
            pressed = (controls.keyboard[SDL_SCANCODE_RALT] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_RALT] ^ second);
            break;
        case ST::key::RCTRL:
            pressed = (controls.keyboard[SDL_SCANCODE_RCTRL] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_RCTRL] ^ second);
            break;
        case ST::key::SEMICOLON:
            pressed = (controls.keyboard[SDL_SCANCODE_SEMICOLON] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_SEMICOLON] ^ second);
            break;
        case ST::key::SLASH:
            pressed = (controls.keyboard[SDL_SCANCODE_SLASH] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_SLASH] ^ second);
            break;
        case ST::key::TAB:
            pressed = (controls.keyboard[SDL_SCANCODE_TAB] ^ first) && (controls_prev_frame.keyboard[SDL_SCANCODE_TAB] ^ second);
            break;
        case ST::key::MOUSELEFT:
            pressed = (controls_prev_frame.mouse_clicks[0] ^ first) && (controls.mouse_clicks[0] ^ second);
            break;
        case ST::key::MOUSEMIDDLE:
            pressed = (controls_prev_frame.mouse_clicks[1] ^ first) && (controls.mouse_clicks[1] ^ second);
            break;
        case ST::key::MOUSERIGHT:
            pressed = (controls_prev_frame.mouse_clicks[2] ^ first) && (controls.mouse_clicks[2] ^ second);
            break;

        case ST::key::CONTROLLER_BUTTON_A:
            pressed = (controller_button_prev_frame.a ^ first) && (controller_buttons.a ^ second);
            break;
        case ST::key::CONTROLLER_BUTTON_B:
            pressed = (controller_button_prev_frame.b ^ first) && (controller_buttons.b ^ second);
            break;
        case ST::key::CONTROLLER_BUTTON_X:
            pressed = (controller_button_prev_frame.x ^ first) && (controller_buttons.x ^ second);
            break;
        case ST::key::CONTROLLER_BUTTON_Y:
            pressed = (controller_button_prev_frame.y ^ first) && (controller_buttons.y ^ second);
            break;
        case ST::key::CONTROLLER_BUTTON_SELECT:
            pressed = (controller_button_prev_frame.select ^ first) && (controller_buttons.select ^ second);
            break;
        case ST::key::CONTROLLER_BUTTON_START:
            pressed = (controller_button_prev_frame.start ^ first) && (controller_buttons.start ^ second);
            break;
        case ST::key::CONTROLLER_BUTTON_LEFTSTICK:
            pressed = (controller_button_prev_frame.left_stick ^ first) && (controller_buttons.left_stick ^ second);
            break;
        case ST::key::CONTROLLER_BUTTON_RIGHTSTICK:
            pressed = (controller_button_prev_frame.right_stick ^ first) && (controller_buttons.right_stick ^ second);
            break;
        case ST::key::CONTROLLER_BUTTON_LEFTSHOULDER:
            pressed = (controller_button_prev_frame.left_shoulder ^ first) && (controller_buttons.left_shoulder ^ second);
            break;
        case ST::key::CONTROLLER_BUTTON_RIGHTSHOULDER:
            pressed = (controller_button_prev_frame.right_shoulder ^ first) && (controller_buttons.right_shoulder ^ second);
            break;
        case ST::key::CONTROLLER_BUTTON_DPAD_UP:
            pressed = (controller_button_prev_frame.dpad_up ^ first) && (controller_buttons.dpad_up ^ second);
            break;
        case ST::key::CONTROLLER_BUTTON_DPAD_LEFT:
            pressed = (controller_button_prev_frame.dpad_left ^ first) && (controller_buttons.dpad_left ^ second);
            break;
        case ST::key::CONTROLLER_BUTTON_DPAD_RIGHT:
            pressed = (controller_button_prev_frame.dpad_right ^ first) && (controller_buttons.dpad_right ^ second);
            break;
        case ST::key::CONTROLLER_BUTTON_DPAD_DOWN:
            pressed = (controller_button_prev_frame.dpad_down ^ first) && (controller_buttons.dpad_down ^ second);
            break;
        case ST::key::UNKNOWN:
            pressed = false;
            break;
    }
    return pressed;
}
